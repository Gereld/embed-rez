#include "path.h"

#include <array>
#include <algorithm>

namespace path {

	void breadth_first_file_scan( const std::filesystem::path & location, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::directory_entry & )> filter ) {
		breadth_first_traversal( location, [&]( const std::filesystem::directory_entry & entry ) {
			if( !filter || filter( entry ) ) {
				result.push_back( entry.path() );
			}
		} );
	}

	void depth_first_file_scan( const std::filesystem::path & location, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::directory_entry & )> filter ) {
		depth_first_traversal( location, [&]( const std::filesystem::directory_entry & entry ) {
			if( !filter || filter( entry ) ) {
				result.push_back( entry.path() );
			}
		} );
	}

	std::filesystem::path make_relative( const std::filesystem::path & path, const std::filesystem::path & branch ) {
		// create absolute paths
		std::filesystem::path p = std::filesystem::absolute( path );
		std::filesystem::path b = std::filesystem::absolute( branch );

		auto mr = std::mismatch( p.begin(), p.end(),  b.begin(), b.end() );

		if( mr.first == p.begin() ) {
			return path;
		}

		std::filesystem::path result;

		for( auto i = mr.second; i != b.end(); ++i ) {
			result /= "..";
		}

		for( auto i = mr.first; i != p.end(); ++i ) {
			result /= *i;
		}

		return result;
	}

	// stackoverflow : nijansen
	bool copy_directory(
		std::filesystem::path const & source,
		std::filesystem::path const & destination, std::filesystem::copy_options option ) 
	{
		// Check whether the function call is valid
		if( !std::filesystem::exists( source ) || !std::filesystem::is_directory( source ) ) {
			//std::cerr << "Source directory " << source.string() << " does not exist or is not a directory." << '\n';
			return false;
		}

		if( !std::filesystem::exists( destination ) ) {
			// Create the destination directory
			if( !std::filesystem::create_directory( destination ) ) {
				//std::cerr << "Unable to create destination directory" << destination.string() << '\n';
				return false;
			}
		}

		// Iterate through the source directory
		std::filesystem::directory_iterator end;
		for( std::filesystem::directory_iterator i( source ); i != end; ++i ) {
			std::filesystem::path current( i->path() );
			if( std::filesystem::is_directory( current ) ) {
				// Found directory: Recursion
				if( !copy_directory( current, destination / current.filename(), option ) ) {
					return false;
				}
			}
			else {
				// Found file: Copy
				std::filesystem::copy_file( current, destination / current.filename(), option );
			}
		}

		return true;
	}

	void list_directory( const std::filesystem::path & directory, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::path & )> filter ) {
		std::filesystem::directory_iterator end;
		for( std::filesystem::directory_iterator i{ directory }; i != end; ++i ) {
			std::filesystem::path entry_path = *i;

			if( !filter || filter( entry_path ) ) {
				result.emplace_back( entry_path );
			}
		}
	}

	std::vector<std::filesystem::path> list_directory( const std::filesystem::path & directory, std::function<bool( const std::filesystem::path & )> filter ) {
		std::vector<std::filesystem::path> files;
		list_directory( directory, files, filter );
		return files;
	}

	void reset_directory( const std::filesystem::path & directory ) {
		if( std::filesystem::exists( directory ) ) {
			std::filesystem::remove_all( directory );
		}

		std::filesystem::create_directories( directory );
	}

	std::filesystem::path change_extension( const std::filesystem::path & p, const std::filesystem::path & new_extension ) {
		std::filesystem::path new_p( p );
		new_p.replace_extension( new_extension );
		return new_p;
	}

} // namespace path

