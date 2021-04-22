// std
#include <cstdio>
#include <thread>
#include <chrono>
// third party
#include <gtest/gtest.h>
// project
#include <window/window.hpp>
#include <imagesearch/screenshot.hpp>
#include <task/command.hpp>
#include <control/control.hpp>
#include <task/common.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\"
#define PATH(p) BASE p

struct Size {
    int width;
    int height;

    void print()
    {
        std::printf("size: %d x %d\n", width, height);
    }
};

Size get_rect_size(RECT& rect)
{
    return Size{rect.right - rect.left,
                rect.bottom - rect.top};
}

void screenshot_zx(const char* name)
{
    auto wanmei_platform = autozhuxian::find_window("诛仙3");
    ASSERT_TRUE(wanmei_platform);
    ASSERT_TRUE(wanmei_platform->handle());

    Size win_size = get_rect_size(wanmei_platform->window_info().rcClient);
    win_size.print();

    cv::Mat screenshot = autozhuxian::screenshot_region(wanmei_platform->handle(),
                                                        autozhuxian::RegionOfInterest{0, 0, win_size.width, win_size.height});
    cv::imwrite(name, screenshot);
    autozhuxian::click();
}

TEST(window, close_window)
{
    screenshot_zx(PATH("tmp\\screen.png"));

    // auto zx = autozhuxian::find_window("诛仙3");
    // ASSERT_TRUE(zx);
    // SetForegroundWindow(zx->handle());
}