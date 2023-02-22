#include <iostream>
#include <windows.h>
#include <libloaderapi.h>

int main(int argc, char *argv[]) {
  char *exec_name = argv[0];
  if (char *b = strrchr(exec_name, '\\')) {
    exec_name = b + 1;
  }

  if (argc == 1) {
    std::cout << "\noverview: find a dynamic library"
              << "\n\n"
              << "usage: " << exec_name
              << " <lib>\n"
              << std::endl;
    return 0;
  }

  const char *name = argv[1];
  auto mod = LoadLibraryA(name);

  if (mod == nullptr) {
    std::cerr << "library named " << name << " not found" << std::endl;
    return -1;
  }

  WCHAR chars[MAX_PATH] = {};
  auto read = GetModuleFileNameW(mod, chars, MAX_PATH);
  auto std_out = GetStdHandle(STD_OUTPUT_HANDLE);

  DWORD written;
  BOOL success = WriteConsoleW(std_out, chars, read, &written, nullptr);

  if (!success) {
    return -1;
  }
}