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

/**
 * @brief 在source图片中找到templ的位置
 * 
 * @param source 搜索的图片
 * @param templ 匹配的图片
 * @param method 算法
 * @return cv::Point 对应位置的左上角坐标（相对source图片）
 */
cv::Point match_template(const cv::Mat& source,
                         const cv::Mat& templ,
                         int method = cv::TM_SQDIFF);

/**
 * @brief 在source图片中找到templ的位置（带mask）
 * 
 * @param source 搜索的图片
 * @param templ 匹配的图片
 * @param mask 匹配图片的mask，需要和templ尺寸一致
 * @param method 算法
 * @return cv::Point 对应位置的左上角坐标（相对source图片）
 */
cv::Point match_template_with_mask(const cv::Mat& source,
                                   const cv::Mat& templ,
                                   const cv::Mat& mask,
                                   int method = cv::TM_SQDIFF);

};  // namespace autozhuxian

#endif