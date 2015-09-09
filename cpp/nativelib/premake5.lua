workspace "nativelib"
    configurations { "Release", "Debug", "ReleaseShared", "ReleaseStatic" }
    filter "*Release*"
        defines { "NDEBUG" }
        optimize "On"
    filter "*Debug*"
        defines { "DEBUG" }
        flags { "Symbols" }

project "echo_shared"
    kind "SharedLib"
    language "C++"
    
    files { "echo.h", "echo.cpp", "echo_p.h", "echo_p.cpp", "version.cpp" }

    filter "Release or *Shared"
        kind "SharedLib"
        defines { "NATIVELIB_ECHO_BUILD_SHARED" }
    filter "*Static"
        kind "StaticLib"
        defines { "NATIVELIB_ECHO_BUILD_STATIC" }
