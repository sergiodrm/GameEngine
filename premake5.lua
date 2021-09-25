workspace "VoltEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug", "Release", "Dist"
    }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "VoltEngine/lib/GLFW/include"
IncludeDir["glad"] = "VoltEngine/lib/glad/include"
IncludeDir["glm"] = "VoltEngine/lib/glm"

group "Dependencies"
	include "VoltEngine/lib/GLFW"
	include "VoltEngine/lib/glad"

group "Engine"

include "VoltEngine"

group "Game"
include "Sandbox"