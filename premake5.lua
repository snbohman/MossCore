project "mossCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "build/%{cfg.buildcfg}"

    files { "src/**.cpp" }

    includedirs {
        "include",
        "external/catch2",
        "external/entt/single_include",
        "external/spdlog/include"
    }

    filter "system:linux"
        links { "pthread" } -- Maybe remove

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"
