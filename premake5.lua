dofileopt "default.lua"

-- You can search 'token' on premake's wiki to learn about the %{token} notation.	

newoption {
    trigger = "to",
    value   = "path",
    description = "Set the output location for the generated files"
}

workspace "embed-rez-wks"
	location		( _OPTIONS["to"] )
	configurations 	{ "Release" , "Debug" }
	platforms		{ "x64" }
	cppdialect		"C++20"
	startproject	"example"

	filter { "configurations:Debug" }
		optimize 	"Debug"
		symbols 	"On"

	filter { "configurations:Release" }
		optimize 		"Full"
		symbols 		"Off"
		staticruntime	"On"

include "embed-rez/embed-rez.lua"
	location			"%{wks.location}"
	postbuildcommands 	{ "{COPYFILE} %{cfg.buildtarget.relpath} %{wks.location}/bin/embed-rez.exe" }

	-- Not needed for the build but we add this file for convenient access in Visual Studio.
	files { "premake5.lua" }

project "resources"
	kind		"Utility"
	dependson	{ "embed-rez" }

	files 		{ "example/res.xml" }

	filter 'files:**.xml'
		buildmessage '** Compiling %{file.relpath} **'
		buildinputs		{ "%{wks.location}/bin/embed-rez.exe" }
		buildcommands	{
			'%{wks.location}/bin/embed-rez.exe "%{file.relpath}" %{cfg.basedir}/example/res.inc',
			-- It's not necessary to touch the wrapper cpp file. 
			-- I just leave this here as a reminder on how to use premake's touch utilty.
			-- '{TOUCH} ../example/res.cpp'
		}
		buildoutputs	{ '%{cfg.basedir}/example/res.inc' }

include "example/example.lua"
	location	"%{wks.location}"
	dependson 	{ "embed-rez", "resources" }


