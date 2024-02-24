#include <iostream>
#include <span>

namespace my_rez {
	std::span<unsigned char> get_resource( const char * );
	std::span<std::pair<const char *, std::span<unsigned char>>> get_table();
}

int main( int argc, char * argv[] ) {
	// iterate over all the records
	auto table = my_rez::get_table();
	for( auto record : table ) {
		std::cout << record.first << " " << record.second.size() << "\n";
	}

	std::cout << "\n";

	// access through id
	std::span<unsigned char> p = my_rez::get_resource( "some_image" );
	if( !p.empty() ) {
		const unsigned char * d = p.data();
		std::cout << "some_image "
				  << " " << p.size() << "\n";
	}

	// access through path
	p = my_rez::get_resource( "res/img.jpg" );
	if( !p.empty() ) {
		const unsigned char * d = p.data();
		std::cout << "res/img.jpg "
				  << " " << p.size() << "\n";
	}

	// access an item in a subfolder
	p = my_rez::get_resource( "res/subfolder/A.txt" );
	if( !p.empty() ) {
		const unsigned char * d = p.data();
		std::cout << "res/subfolder/A.txt "
				  << " " << p.size() << "\n";
	}

	return 0;
}