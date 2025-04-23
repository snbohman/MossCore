workspace "MossCore"
    configurations { "debug", "release" }

    language "C++"
    cppdialect "C++20"
    architecture "x86_64"
    toolset "clang"

    location "scripts"
    flags { "MultiProcessorCompile" }

    project "Core"
        kind "StaticLib"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "build/%{cfg.buildcfg}"

        files { "src/**.cpp" }

        includedirs {
            "include",
            "external/entt/single_include",
            "external/spdlog/include"
        }

        filter "configurations:debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:release"
            defines { "NDEBUG" }
            optimize "On"

    project "Tests"
        kind "ConsoleApp"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "build/%{cfg.buildcfg}"

        files { "tests/**.cpp" }
        links { "Core" }

        includedirs {
            "include",
            "tests/player-enemy/include",
            "external/doctest",
            "external/entt/single_include",
            "external/spdlog/include"
        }

        filter "configurations:debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:release"
            defines { "NDEBUG" }
            optimize "On"
