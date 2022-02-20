#ifdef _WIN32
#include <Windows.h>
auto main(int argc, char **argv) -> int;
auto WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nShowCmd)
    -> int {
  return main(__argc, __argv);
}
#endif