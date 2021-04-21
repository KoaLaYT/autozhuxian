// std
#include <cstdio>
#include <vector>
// project
#include <window/window.hpp>
#include "task.hpp"

using ZXWindow = autozhuxian::Window;

struct ZXWindowManager {
    std::vector<ZXWindow> windows;

    static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
    {
        CHAR title[1024];
        int  length = GetWindowTextA(hwnd, title, sizeof(title));

        // 有的无窗口应用还是会有hwnd，在这里过滤掉
        if (!IsWindowVisible(hwnd) ||
            length == 0 ||
            std::strcmp(title, "Program Manager") == 0) {
            return TRUE;
        }

        std::vector<ZXWindow>& result = *reinterpret_cast<std::vector<ZXWindow>*>(lParam);
        if (std::strcmp(title, "诛仙3") == 0) {
            result.push_back(ZXWindow{"诛仙3", hwnd});
        }

        return TRUE;
    }

    ZXWindowManager()
    {
        EnumWindows(EnumWindowCb, reinterpret_cast<LPARAM>(&windows));
    }
};

int main()
{
    LaunchTask launch;
    launch.run();

    ZXWindowManager zxwm;

    for (auto& win : zxwm.windows) {
        LoginTask task;
        task.run(win);
    }

    return 0;
}