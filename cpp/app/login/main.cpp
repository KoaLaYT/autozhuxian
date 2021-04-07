#include <window/window.hpp>
#include <control/control.hpp>
#include <imagesearch/screenshot.hpp>
#include <cstdio>
#include <thread>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\tmp\\"
#define PATH(p) BASE p

static bool open_platform();
static autozhuxian::ScreenshotRegion get_desktop_region(HWND hwnd);

int main()
{
    char* platform = "完美游戏平台";
    HWND platform_hwnd = autozhuxian::find_window(platform);
    if (!platform_hwnd) {
        std::printf("未找到窗口，尝试打开窗口\n");
        open_platform();
    }

    return 0;
}

static bool open_platform()
{
    using namespace std::chrono_literals;

    autozhuxian::press(VK_LWIN);
    // waiting for the window shows
    std::this_thread::sleep_for(2000ms);

    HWND desktop_hwnd = GetDesktopWindow();
    auto desktop_region = get_desktop_region(desktop_hwnd);
    auto mat = autozhuxian::screenshot_region(desktop_hwnd, desktop_region.left_half());
    cv::imwrite(PATH("platform_desktop.png"), mat);

    return false;
}

static autozhuxian::ScreenshotRegion get_desktop_region(HWND hwnd)
{
    RECT window_rect;
    GetClientRect(hwnd, &window_rect);
    return autozhuxian::ScreenshotRegion{window_rect};
}