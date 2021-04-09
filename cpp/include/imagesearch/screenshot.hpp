#ifndef KOALAYT_SCREENSHOT_HPP_20210407
#define KOALAYT_SCREENSHOT_HPP_20210407

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

namespace autozhuxian {

/**
 * @brief ROI 窗口的某一区域，也即搜索的source区域
 * 
 */
struct RegionOfInterest {
    int x;
    int y;
    int width;
    int height;
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