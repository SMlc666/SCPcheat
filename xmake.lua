add_rules("mode.debug", "mode.release")
add_requires("microsoft-detours 2023.6.8")
add_requires("imgui 1.91.8", {configs = {dx11 = true,win32 = true}})
add_requires("spdlog 1.15.2")
add_requires("gtest 1.16.0")
target("scpCheat")
    set_kind("shared")
    set_languages("c++23")
    add_packages("microsoft-detours", "imgui", "spdlog")
    -- 排除测试文件，防止被编译进 DLL
    add_files("src/**.cpp|test/test_*.cpp")
    add_includedirs("src","src/include")
    add_links("user32")

-- 自动为每个 test_*.cpp 创建测试 target
for _, file in ipairs(os.files("src/test/test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_files("src/test/" .. name .. ".cpp")
        add_files("src/hook/hook.cpp")
        add_files("src/log/log.cpp")
        add_packages("gtest", "microsoft-detours","spdlog")
        add_includedirs("src", "src/include")
        add_tests("default")
end
    