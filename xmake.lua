add_rules("mode.debug", "mode.release")
add_requires("safetyhook 0.6.5")
add_requires("imgui 1.91.8", {configs = {dx11 = true,win32 = true}})
add_requires("spdlog 1.15.2")
add_requires("gtest")
add_requires("eventpp 0.1.3")




target("scpCheat")
    set_kind("shared")
    set_languages("c++23")
    add_packages("safetyhook", "imgui", "spdlog","eventpp")
    add_files("src/**.cpp|test/test_*.cpp")
    add_includedirs("src","src/include")
    add_links("user32")
    if is_mode("release") then
        remove_files("src/draw/gui/window/windows/DebugWindow.cpp")
    end
target("test")
    set_kind("binary")
    set_default(false)
    set_languages("c++23")
    add_files("src-test/**.cpp")
    add_files("src/log/log.cpp")
    add_files("src/module/*.cpp")
    add_packages("gtest", "spdlog")
    add_includedirs("src", "src/include")
    add_tests("default")