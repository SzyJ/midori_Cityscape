workspace "Cityscape"
    architecture "x64"
    startproject "Cityscape"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

midori_path = "3rd_party/midori_engine/"
rel_midori_path = ("../" .. midori_path)
include (midori_path .. "premake_include_midori.lua")

project "Cityscape"
    location "Cityscape"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp"
    }

    includedirs {
        (midori_path .. "midori_engine/src"),
        (midori_path .. "midori_engine/3rd_party/glm"),
        (midori_path .. "midori_engine/3rd_party/spdlog/include"),
        (midori_path .. "midori_engine/3rd_party/imgui")
    }

    links {
        "midori_engine"
    }

filter "system:windows"
    systemversion "latest"

    defines {
        "MD_PLATFORM_WINDOWS"
    }

filter "configurations:Debug"
    defines "MD_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "MD_RELEASE"
    runtime "Release"
    optimize "On"

filter "configurations:Dist"
    defines "MD_DIST"
    runtime "Release"
    optimize "On"
