#ifndef KOALAYT_SCREENSHOT_HPP_20210407
#define KOALAYT_SCREENSHOT_HPP_20210407

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

namespace autozhuxian {

struct ScreenshotRegion {
    int top;
    int left;
    int width;
    int height;

    ScreenshotRegion left_half()
    {
        return ScreenshotRegion{top, left, width / 2, height};
    }

    ScreenshotRegion left_bottom()
    {
        return ScreenshotRegion{top + height / 2, left, width / 2, height / 2};
    }

    ScreenshotRegion(int t, int l, int w, int h) : top{t},
                                                   left{l},
                                                   width{w},
                                                   height{h}
    {}

    explicit ScreenshotRegion(RECT rect) : top{rect.top},
                                           left{rect.left},
                                           width{rect.right - rect.left},
                                           height{rect.bottom - rect.top}
    {}
};

/**
 * @brief 窗口的某一区域截图
 * 
 * @param hwnd 窗口的handle
 * @param region 要截图的区域
 * @return cv::Mat 
 */
cv::Mat screenshot_region(HWND hwnd, ScreenshotRegion region);

};  // namespace autozhuxian

#endif