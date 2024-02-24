#pragma once

#include <functional>
#include <vector>
#include <filesystem>

#if __has_concepts
#include <concepts>
#endif

namespace path {

#if __has_concepts
	template<std::invocable<const std::filesystem::directory_entry &> OP>
#else
	template<typename OP>
#endif
	void breadth_first_traversal( const std::filesystem::path & location, OP op ) {
		std::vector<std::filesystem::path> directories;

		std::filesystem::directory_iterator end;
		for( std::filesystem::directory_iterator i{ location }; i != end; ++i ) {
			const std::filesystem::directory_entry & entry  = *i;
			std::filesystem::file_status             status = entry.status();
			const std::filesystem::path &            file   = entry.path();

			if( std::filesystem::is_directory( status ) ) {
				directories.push_back( file );
			}

			op( entry );
		}

		for( const std::filesystem::path & dir : directories ) {
			breadth_first_traversal( dir, op );
		};
	}

#if __has_concepts
	template<std::invocable<const std::filesystem::directory_entry &> OP>
#else
	template<typename OP>
#endif
	void depth_first_traversal( const std::filesystem::path & location, OP op ) {
		std::filesystem::directory_iterator end;
		for( std::filesystem::directory_iterator i{ location }; i != end; ++i ) {
			const std::filesystem::directory_entry & entry  = *i;
			std::filesystem::file_status             status = entry.status();
			const std::filesystem::path &            file   = entry.path();

			if( std::filesystem::is_directory( status ) ) {
				depth_first_traversal( file, op );
			}

			op( entry );
		}
	}

	void                               breadth_first_file_scan( const std::filesystem::path & location, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::directory_entry & )> filter = {} );
	void                               depth_first_file_scan( const std::filesystem::path & location, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::directory_entry & )> filter = {} );
	std::filesystem::path              make_relative( const std::filesystem::path & path, const std::filesystem::path & branch );
	bool                               copy_directory( std::filesystem::path const & source, std::filesystem::path const & destination, std::filesystem::copy_options option = std::filesystem::copy_options::none );
	void                               list_directory( const std::filesystem::path & directory, std::vector<std::filesystem::path> & result, std::function<bool( const std::filesystem::path & )> filter = {} );
	std::vector<std::filesystem::path> list_directory( const std::filesystem::path & directory, std::function<bool( const std::filesystem::path & )> filter = {} );
	void                               reset_directory( const std::filesystem::path & directory );
	std::filesystem::path              change_extension( const std::filesystem::path & p, const std::filesystem::path & new_extension );

} // namespace path

