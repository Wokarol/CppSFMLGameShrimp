workspace "SurvivingTheHeat"
    architecture "x64"
    language "C++"

    configurations
    {
        "Debug",
        "Release",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "Game"
    location "game"
    kind "ConsoleApp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.h",
    }
    includedirs { 
        "%{prj.location}/include",
        "%{wks.location}/lib/SFML-2.5.1/include",
        "%{wks.location}/lib/json",
    }

    libdirs { 
        "lib/SFML-2.5.1/lib",
    }
    links { 
        "opengl32.lib", 
        "freetype.lib", 
        "winmm.lib", 
        "gdi32.lib", 
        "openal32.lib", 
        "flac.lib", 
        "vorbisenc.lib", 
        "vorbisfile.lib", 
        "vorbis.lib", 
        "ogg.lib", 
        "ws2_32.lib",
    }
    postbuildcommands {
        "{DELETE} %{cfg.targetdir}/assets/**",
        "{COPY} %{prj.location}/assets %{cfg.targetdir}/assets/",
    }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

        defines {
            "DEBUG",
        }

        links { 
            "sfml-audio-s-d.lib", 
            "sfml-graphics-s-d.lib", 
            "sfml-network-s-d.lib", 
            "sfml-system-s-d.lib", 
            "sfml-window-s-d.lib", 
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "On"

        defines {
            "RELEASE",
        }

        links { 
            "sfml-audio-s.lib", 
            "sfml-graphics-s.lib",
            "sfml-network-s.lib", 
            "sfml-system-s.lib", 
            "sfml-window-s.lib",
        }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines {
            "GAME_PLATFORM_WINDOWS", 
            "SFML_STATIC",
        }
