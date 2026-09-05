-- Used for Github Actions unit testing
local is_standalone = _ACTION and not _G.__root_workspace_defined

if is_standalone then
    _G.__root_workspace_defined = true

    workspace("MossDivided")
        language "C++"
        cppdialect "C++20"
        architecture "x86_64"
        toolset "clang"

        location "scripts"
        flags { "MultiProcessorCompile" }
        configurations { "debug", "release" }
end

    project "mossCoreTests"
        kind "ConsoleApp"
        location "scripts"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "build/%{cfg.buildcfg}/%{prj.name}"

        files { "tests/**.cpp" }
        includedirs {
            "include",
            "tests/include"
        }

        filter "configurations:debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:release"
            defines { "NDEBUG" }
            optimize "On"
