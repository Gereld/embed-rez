dofileopt "default.lua"

workspace "embed-rez" 
	configurations { "Release" , "Debug" }
	cppdialect "C++20"

	filter { "configurations:Debug" }
		optimize 	"Debug"
		symbols 	"On"

	filter { "configurations:Release" }
		optimize 	"Full"
		symbols 	"Off"

include "embed-rez.lua"

