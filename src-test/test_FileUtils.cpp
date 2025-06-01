#include "gtest/gtest.h"
#include "utils/FileUtils.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST(FileUtilsTest, GetProcessExecutablePath_ReturnsValidPath) {
    fs::path path = zr::get_process_executable_path();
    EXPECT_FALSE(path.empty());
    EXPECT_TRUE(path.is_absolute());
}

TEST(FileUtilsTest, GetProcessExecutablePath_FileExists) {
    fs::path path = zr::get_process_executable_path();
    EXPECT_TRUE(fs::exists(path));
    EXPECT_TRUE(fs::is_regular_file(path));
}

TEST(FileUtilsTest, GetProcessExecutablePath_HasCorrectExtension) {
    fs::path path = zr::get_process_executable_path();
    EXPECT_EQ(path.extension().string(), ".exe");
}