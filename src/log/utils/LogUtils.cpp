#include "LogUtils.hpp"
#include <Windows.h>
#include <iostream>
#include <vector>

namespace zr {
std::filesystem::path get_process_executable_path() {
  std::vector<char> buffer(MAX_PATH);
  DWORD size = 0;

  // 循环获取路径，处理路径可能超过 MAX_PATH 的情况
  while (true) {
    size = GetModuleFileNameA(NULL, // NULL 表示获取当前进程的可执行文件路径
                              buffer.data(), buffer.size());

    if (size == 0) {
      // 获取失败
      DWORD error = GetLastError();
      std::cerr << "获取模块文件名失败，错误码: " << error << std::endl;
      return std::filesystem::path(); // 返回空路径表示失败
    }

    if (size < buffer.size()) {
      // 缓冲区足够大，成功获取
      break;
    }

    // 缓冲区太小，需要更大的缓冲区
    buffer.resize(buffer.size() * 2);
  }

  // 从缓冲区创建 std::string，然后转换为 std::filesystem::path
  std::string processPath(buffer.data(), size);
  return std::filesystem::path(processPath);
}
} // namespace zr