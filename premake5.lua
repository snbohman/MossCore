workspace "moss"
    configurations { "debug", "release" }
    architecture "x86_64"
    location "scripts"

    project "moss"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "build/%{cfg.buildcfg}"

        files { "moss/src/**.cpp" }
        includedirs { "moss/include", "entt" }
        links { "raylib", "fmt" }

        filter "configurations:debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:release"
            defines { "NDEBUG" }
            optimize "On"
