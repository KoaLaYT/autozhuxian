#ifndef KOALAYT_SCREENSHOT_HPP_20210407
#define KOALAYT_SCREENSHOT_HPP_20210407

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

namespace autozhuxian {

/**
 * @brief ROI 窗口的某一区域
 * 
 */
struct RegionOfInterest {
    int x;
    int y;
    int width;
    int height;

    bool is_whole() { return width == 0 || height == 0; }

    // 一个特殊的值，代表整个屏幕都是ROI
    static RegionOfInterest whole;
};

/**
 * @brief 窗口的某一区域截图
 * 
 * @param hwnd 窗口的handle
 * @param region 要截图的区域
 * @return cv::Mat 
 */
cv::Mat screenshot_region(HWND hwnd, RegionOfInterest roi);

};  // namespace autozhuxian

#endif