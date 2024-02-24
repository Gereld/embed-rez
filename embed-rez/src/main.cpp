#include "path/path.h"

#include <array>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

#define PUGIXML_HEADER_ONLY
#include "pugixml.hpp"

namespace fs = std::filesystem;

void output( std::ostream & os, const fs::path & filename ) {
	std::ifstream is( filename.native().c_str(), std::ios::binary );
	bool bFirst = false;
	std::array<unsigned char, 80> buffer;
	std::streamsize size = 0;
	do {
		is.read( reinterpret_cast<char *>( buffer.data() ), buffer.size() );
		size = is.gcount();
		if( size > 0 ) {
			os << '\t';
			for( int i = 0; i < static_cast<int>( size ); ++i ) {
				os << static_cast<unsigned int>( buffer[i] ) << ',';
			}
			os << '\n';
		}
	} while( size > 0 );
}

struct Record {
	std::string		m_id;
	unsigned int	m_num;
	unsigned int	m_filesize;
	fs::path		m_path;
};

void usage() {
	std::cout << "usage : embed-rez resource_file output_file" << std::endl;
}

const char * header = R"x(
#include <cstring>
#include <algorithm>
#include <array>
#include <span>
)x";

const char * content = R"x(
using Resource = std::span<unsigned char>;
using Record = std::pair<const char *, Resource>;
using std::make_pair;

static constexpr size_t size = {0};

static std::array<Record, size> table = {{
{1}	}};

std::span<Record> get_table() {{
	return table;
}}

Resource get_resource( const char * id ) {{
	auto pos = std::lower_bound( table.begin(), table.end(), id, []( const Record & r, const char * key ) {{
		return strcmp( r.first, key ) < 0;
	}} );

	if( pos < table.end() ) {{
		if( strcmp( pos->first, id ) == 0 ) {{
			return pos->second;
		}}
	}}

	return {{}};
}}
)x";

std::string_view trim( std::string_view in ) {
	auto left = in.begin();
	for( ;; ++left ) {
		if( left == in.end() ) {
			return std::string_view();
		}
		if( !isspace( *left ) ) {
			break;
		}
	}
	auto right = in.end() - 1;
	for( ; right > left && isspace( *right ); --right )
		;
	auto d = std::distance( left, right ) + 1;
	return std::string_view( left, right + 1 );
}

int main( int argc, char * argv[] ) {
	std::cout << "embed-rez 1.0 resource compiler." << std::endl;
	if( argc < 3 ) {
		usage();
		return 1;
	}

	std::string input_filename = argv[1];
	fs::path input_path( input_filename );
	if( !fs::exists( input_path ) ) {
		std::cout << "error : Can't xml find file \"" << input_filename << "\"." << std::endl;
		return 1;
	}

	fs::path root_path = input_path.parent_path();

	std::string output_filename = argv[2];

	pugi::xml_document xml;
	xml.load_file( input_filename.c_str() );

	std::vector<Record> records;

	for( auto && node : xml.children( "resource" ) ) {
		const char * str_path = node.text().as_string();

		auto str_view = trim( std::string_view( str_path ) );
		std::string str_cleaned( str_view.begin(), str_view.end() );

		fs::path path_path = root_path / str_cleaned;
		if( !fs::exists( path_path ) ) {
			std::cout << "error : Can't find resource file \"" << path_path.string() << "\"." << std::endl;
			return 1;
		}

		using Entry = std::pair<std::string, fs::path>;
		std::vector<Entry> entries;
		
		if( fs::is_directory( path_path ) ) {
			std::vector<fs::path> files;
			path::breadth_first_file_scan( path_path, files, []( const fs::path & path ) {
				return !fs::is_directory( path );
			} );

			for( const fs::path & file : files ) {
				entries.emplace_back( std::make_pair( std::string(), file ) );
			}
		} 
		else {
			std::string str_id;
			pugi::xml_attribute id_attr = node.attribute( "id" );
			if( id_attr ) {
				str_id = id_attr.value();
			}

			entries.emplace_back( std::make_pair( std::move( str_id ), path_path ) );
		}

		for( const Entry & entry : entries ) {
			const fs::path & file = entry.second;

			std::string str_id;
			if( entry.first.empty() ) {
				str_id = path::make_relative( file, root_path ).string();
				std::replace( str_id.begin(), str_id.end(), '\\', '/' );
			}
			else {
				str_id = entry.first;
			}

			unsigned int filesize = static_cast<unsigned int>( fs::file_size( file ) );

			records.emplace_back( Record{ std::move( str_id ), static_cast<unsigned int>( records.size() ), filesize, file } );
		}
	}

	std::ofstream os( output_filename.c_str() );
	std::ios_base::sync_with_stdio( false );
	os << header << '\n';

	pugi::xml_node ns = xml.child( "namespace" );
	if( ns ) {
		os << std::format( "namespace {} {{", ns.text().as_string() ) << '\n';
	}

	for( int i = 0; i < static_cast<int>( records.size() ); ++i ) {
		const Record & r = records[i];

		if( r.m_filesize != 0 ) {
			// fileSize + 1 for the additional zero
			os << "static unsigned char res_" << i << "[" << r.m_filesize << " + 1] = { " << '\n';
			output( os, r.m_path );

			// We always add zero at the end so that text files can be used as C strings
			os << '\t' << "0" << '\n';

			os << "};" << '\n';
			os << '\n';
		}
		else {
			os << "unsigned char res_" << i << "[1] = { 0 };" << '\n';
			os << '\n';
		}
	}

	std::sort( records.begin(), records.end(), []( const auto & lhs, const auto & rhs ) {
		return lhs.m_id < rhs.m_id;
	} );

	std::string buffer;
	for( const Record & r : records ) {
		std::format_to( std::back_inserter( buffer ), "\t\tmake_pair( \"{}\", Resource( res_{}, {} ) ),\n", r.m_id, r.m_num, r.m_filesize );
	}

	std::string str = std::vformat( content, std::make_format_args( records.size(), buffer.data() ) );

	os << str << '\n';

	if( ns ) {
		os << std::format( "}} // namespace {}", ns.text().as_string() ) << '\n';
	}

	std::cout << "done." << '\n';

	return 0;
}
