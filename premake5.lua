workspace "Sarabi"
    configurations { "Debug", "Release" }

project "Sarabi"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"

    files 
    { 
        "src/**.asm",
        "src/**.h", 
        "src/**.c"
    }