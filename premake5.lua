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
IncludeDir["spdlog"] = "VoltEngine/lib/spdlog/include"
IncludeDir["stb_image"] = "VoltEngine/lib/stb_image"
IncludeDir["imgui"] = "VoltEngine/lib/imgui"
IncludeDir["tinyobjloader"] = "VoltEngine/lib/tinyobjloader"
IncludeDir["ImGuizmo"] = "VoltEngine/lib/ImGuizmo"
IncludeDir["lua"] = "VoltEngine/lib/lua/include"

group "Dependencies"
	include "VoltEngine/lib/GLFW"
	include "VoltEngine/lib/glad"
	include "VoltEngine/lib/imgui"
	include "VoltEngine/lib/tinyobjloader"
	

group "Engine"

project "VoltEngine"
    location "VoltEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}-bin")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}-binobj")

    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/lib/glm/glm/**.hpp",
		"%{prj.name}/lib/glm/glm/**.inl",
        "%{prj.name}/lib/stb_image/**.h",
        "%{prj.name}/lib/stb_image/**.cpp",
		"%{prj.name}/lib/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/lib/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/lib/lua/**.h",
		"%{prj.name}/lib/lua/**.cpp",
	}

	includedirs
	{
		"VoltEngine/src",
		"VoltEngine/lib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.tinyobjloader}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.lua}",
	}

	links
	{
		"GLFW",
		"Glad",
        "ImGui",
        "tinyobjloader",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		"VOLT_PLATFORM_WINDOWS",
		"GLFW_INCLUDE_NONE", 
        "VOLT_ENGINE=1"
		}


	filter "configurations:Debug"
		symbols "on"
		defines {"VOLT_DEBUG"}

	filter "configurations:Release"
		optimize "on"
		defines {"VOLT_RELEASE"}

	filter "configurations:Dist"
		optimize "on"
		defines {"VOLT_DIST"}

project "VoltEditor"
    location "VoltEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}-bin")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}-binobj")

    files
	{
		"VoltEditor/src/**.h",
		"VoltEditor/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/VoltEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.ImGuizmo}",
	}

	links
	{
		"VoltEngine",
        "ImGui"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		"VOLT_PLATFORM_WINDOWS",
        "VOLT_EDITOR=1"
		}


	filter "configurations:Debug"
		symbols "on"
		defines {"VOLT_DEBUG"}

	filter "configurations:Release"
		optimize "on"
		defines {"VOLT_RELEASE"}

	filter "configurations:Dist"
		optimize "on"
		defines {"VOLT_DIST"}

group "Game"
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.name}/bin/" .. outputdir .. "/%{prj.name}-bin")
    objdir ("%{wks.name}/bin-int/" .. outputdir .. "/%{prj.name}-binobj")

    files
    {
        "Sandbox/src/**.h",
        "Sandbox/src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/VoltEngine/src",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "VoltEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
        "VOLT_PLATFORM_WINDOWS",
        "VOLT_ENGINE=0"
        }


    filter "configurations:Debug"
		symbols "on"
		defines {"VOLT_DEBUG"}

	filter "configurations:Release"
		optimize "on"
		defines {"VOLT_RELEASE"}

	filter "configurations:Dist"
		optimize "on"
		defines {"VOLT_DIST"}