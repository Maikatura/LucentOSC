include "dependencies.lua"

workspace "LucentOsc"
    architecture "x64"
    startproject "LucentOsc"
    configurations
    {
        "Debug",
        "Release",
		"Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Application"
include "App"

group "External"
include "External/dpp"
include "External/imgui"
