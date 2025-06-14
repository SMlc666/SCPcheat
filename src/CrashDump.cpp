#include "CrashDump.hpp"
#include "utils/FileUtils.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>
#include <tchar.h>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace zr {

LONG WINAPI ExceptionHandler(_EXCEPTION_POINTERS *exceptionInfo) {
  auto exePath = get_process_executable_path();
  auto dumpDir = exePath.parent_path() / "dumps";
  std::filesystem::create_directories(dumpDir);

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm;
  localtime_s(&tm, &time);

  std::ostringstream oss;
  oss << "crash_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".dmp";
  auto dumpPath = dumpDir / oss.str();

  HANDLE file = CreateFileW(dumpPath.c_str(), GENERIC_WRITE, 0, nullptr,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (file != INVALID_HANDLE_VALUE) {
    MINIDUMP_EXCEPTION_INFORMATION info;
    memset(&info, 0, sizeof(info));
    info.ThreadId = GetCurrentThreadId();
    info.ExceptionPointers = exceptionInfo;
    info.ClientPointers = FALSE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file,
                      MiniDumpNormal, exceptionInfo ? &info : nullptr, nullptr,
                      nullptr);

    CloseHandle(file);
  }

  return EXCEPTION_CONTINUE_SEARCH;
}

void setupCrashDump() { SetUnhandledExceptionFilter(ExceptionHandler); }

} // namespace zr