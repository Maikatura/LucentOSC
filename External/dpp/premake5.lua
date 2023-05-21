project "DPP"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "Off"
debugdir "%{wks.location}/lib"
configurations { Debug, Release}
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}
targetdir ("%{wks.location}/Temp/lib/")
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
}

libdirs 
{ 
	"%{wks.location}/lib"
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451" --arithmetic overflow
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"_WINSOCKAPI_",
	"NOMINMAX",
	"DPP_STATIC"
}

includedirs{
    "./include/dpp-10.0"
}

externalincludedirs {
	
}

externalwarnings "Off"

links
{
	"dpp.lib"
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp"
}

flags
{
    "MultiProcessorCompile",
    "FatalCompileWarnings"
}




filter "system:windows"
    systemversion "latest"
    
filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"
	prebuildcommands { "copy /y \"%{wks.location}External\\dpp\\debug\\bin\\*.dll\" \"%{wks.location}bin\"" }
	libdirs 
	{ 
		"%{Libs.DPP}/debug/lib/dpp-10.0/"
	}

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
	prebuildcommands { "copy /y \"%{wks.location}External\\dpp\\release\\bin\\*.dll\" \"%{wks.location}bin\"" }
	libdirs 
	{ 
		"%{Libs.DPP}/release/lib/dpp-10.0/"
	}
    
filter {}
