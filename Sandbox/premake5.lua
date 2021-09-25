project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.name}/bin/" .. outputdir .. "/%{prj.name}-bin")
    objdir ("%{wks.name}/bin-int/" .. outputdir .. "/%{prj.name}-binobj")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "%{wks.location}/VoltEngine/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "VoltEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
        "VOLT_PLATFORM_WINDOWS"
        }


    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"

    filter "configurations:Dist"
        optimize "on"