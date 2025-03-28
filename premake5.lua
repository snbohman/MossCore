project "mossCore"
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

project "mossCoreTests"
    kind "ConsoleApp"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "build/%{cfg.buildcfg}"

    files { "tests/**.cpp" }
    includedirs { "external/doctest", "include" }
    links { "mossCore" }
