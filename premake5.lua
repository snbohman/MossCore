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
        objdir "build/%{cfg.buildcfg}/%{prj.name}"

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
        objdir "build/%{cfg.buildcfg}/%{prj.name}"

        files { "tests/**.cpp" }
        links { "Core" }

        includedirs {
            "include",
            "tests/include",
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
