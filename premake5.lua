include "dependencies.lua"

workspace "LucentOsc"
    architecture "x64"
    startproject "App"
    configurations
    {
        "Debug",
        "Release",
		"Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Application"
include "App"
