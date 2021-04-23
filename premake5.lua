workspace "The Game or some shit"
    language "C++"

    configurations
    {
        "Debug",
        "Release",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "Cool Game"
    location "game"
    kind "ConsoleApp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.h",
    }
    includedirs 
    { 
        "%{prj.location}/include" 
    }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"