#include <gtest/gtest.h>

#include <cstdio>

#include <window/window.hpp>
#include <imagesearch/screenshot.hpp>

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

TEST(window, find_window)
{
    auto wanmei_platform = autozhuxian::find_window("诛仙3");
    ASSERT_TRUE(wanmei_platform);
    ASSERT_TRUE(wanmei_platform->handle());

    Size win_size = get_rect_size(wanmei_platform->window_info().rcClient);
    win_size.print();

    cv::Mat screenshot = autozhuxian::screenshot_region(wanmei_platform->handle(),
                                                        autozhuxian::RegionOfInterest{0, 0, win_size.width, win_size.height});
    cv::imwrite("C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\tmp\\screen.png", screenshot);
}