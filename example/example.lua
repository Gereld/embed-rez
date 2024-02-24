project "example"
	kind		"ConsoleApp"
	includedirs	{ "src" } 

	files		{ "src/**.h", "src/**.hpp", "src/**.cpp" }

	-- Not needed for the build but we add this file for convenient access in Visual Studio.
	files		{ "example.lua" }	


