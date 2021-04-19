#ifndef KOALAYT_SCREENSHOT_HPP_20210407
#define KOALAYT_SCREENSHOT_HPP_20210407

// win32
#include <windows.h>
#include <winuser.h>
// third party
#include <opencv2/opencv.hpp>

namespace autozhuxian {

///
/// ROI 窗口的某一区域
/// ---------------------------------------------------------
/// 有时候，我们可以提前判断，一个窗口中只有某一部分是有关键信息的（比如要搜索的目标图片）
/// 可以减少误判，并且提升一定的效率
///
struct RegionOfInterest {
    int x;       // 左上角x
    int y;       // 左上角y
    int width;   // 宽度
    int height;  // 高度

    // ---------------------------------------------------------
    // 判断是否是特殊的ROI
    //
    bool is_whole() { return width == 0 || height == 0; }

    // ---------------------------------------------------------
    // 一个特殊的值，代表整个屏幕都是ROI
    // ?可能用std::optional更好
    //
    static RegionOfInterest whole;
};

///
/// 对窗口的某一区域截图
/// ---------------------------------------------------------
//? roi可能替换成std::optional<ROI>，以代表对整个窗口截图
///
cv::Mat screenshot_region(HWND hwnd, RegionOfInterest roi);

};  // namespace autozhuxian

#endif