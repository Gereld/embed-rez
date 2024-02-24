project "embed-rez"
	kind		"ConsoleApp"
	--targetdir	"%{wks.location}/bin/%{cfg.shortname}"
	includedirs	{ "src", "3rdparty/pugixml/src" } 

	files		{ "src/**.h", "src/**.hpp", "src/**.cpp" }

	-- Not needed for the build but we add this file for convenient access in Visual Studio.
	files		{ "embed-rez.lua" }

