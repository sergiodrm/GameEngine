project "VoltEngine"
    kind "StaticLib"
    language "C++"

    cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.name}/bin/" .. outputdir .. "/%{prj.name}-bin")
	objdir ("%{wks.name}/bin-int/" .. outputdir .. "/%{prj.name}-binobj")

    files
	{
		"src/**.h",
		"src/**.cpp",
		"lib/glm/glm/**.hpp",
		"lib/glm/glm/**.inl",
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		"VOLT_PLATFORM_WINDOWS",
		"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "configurations:Dist"
		optimize "on"