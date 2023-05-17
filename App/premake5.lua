project "App"
location "."
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
staticruntime "Off"
debugdir "%{wks.location}/Bin"
configurations { Debug, Release}
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}
targetdir ("%{wks.location}/Bin/")
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
	"-ignore:4251",
	"-ignore:4275",
	"-ignore:4355",
	"-ignore:4505",
	"-ignore:4244"
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451", --arithmetic overflow
	"2220"
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"_WINSOCKAPI_",
	"NOMINMAX",
	"_CRT_SECURE_NO_WARNINGS",
	"_HAS_EXCEPTIONS=0",
	"_WINSOCK_DEPRECATED_NO_WARNINGS",
	"WIN32",
	"_WINDOWS",
	"WIN32_LEAN_AND_MEAN",
	"_WIN32_WINNT=0x0602",
	"ESP8266",
	"BUILDING_LIBCURL",
	"_CRT_SECURE_NO_DEPRECATE",
	"_CRT_NONSTDC_NO_DEPRECATE"
}

libdirs 
{ 
	"%{wks.location}/lib/",
	"%{Libs.SFML}"
}

includedirs{
    "./src",
	"%{IncludeDirs.ImGui}",
	"%{IncludeDirs.SFML}",
	"%{IncludeDirs.DPP}"
}

externalincludedirs {

}

externalwarnings "Off"

links
{
	"DPP",
	"ImGui",
	
	"ws2_32.lib",
	"Iphlpapi.lib",
	"Userenv.lib",
	"opengl32.lib",
	"winmm.lib",
	"d3d11.lib",
	"dxguid.lib"
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp",
	"**.rc",
	"**.ico",
	"**.APS"
}

flags
{
    "MultiProcessorCompile"
}

filter "system:windows"
    systemversion "latest"
    
filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"
	
filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
	
filter {}
