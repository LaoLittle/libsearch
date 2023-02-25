#include <iostream>
#include <windows.h>
#include <libloaderapi.h>

constexpr int Ok = 0;
constexpr int Err = 1;

#ifdef _AMD64_
#define ARCH "amd64"
#else
#define ARCH "x86"
#endif // __AMD64__


int main(int argc, char *argv[]) {
  char *exec_name = argv[0];
  if (char *b = strrchr(exec_name, '\\')) {
    exec_name = b + 1;
  }

  if (argc != 2) {
    std::cout << "\noverview: find a dynamic library"
              << "\n\n"
              << "usage: " << exec_name << " <lib>\n"
              << std::endl;
    return Ok;
  }

  const char *name = argv[1];
  auto mod = LoadLibraryA(name);

  if (mod == nullptr) {
    auto err = GetLastError();

    std::cerr << "library named \"" << name << "\" ";
    
    switch (err) {
      case ERROR_MOD_NOT_FOUND:
      std::cerr << "not found";
      break;
      case ERROR_BAD_EXE_FORMAT:
      std::cerr << "is found, but not a valid " << ARCH << " library";
      break;
    default:
      std::cerr << "cannot load, reason: " << err;
      break;
    }

    std::cerr << std::endl;

    return Err;
  }

  WCHAR chars[MAX_PATH]{};
  auto read = GetModuleFileNameW(mod, chars, MAX_PATH);
  auto std_out = GetStdHandle(STD_OUTPUT_HANDLE);

  DWORD written;
  BOOL success = WriteConsoleW(std_out, chars, read, &written, nullptr);

  if (!success) {
    return Err;
  }
}