// std
#include <cstdio>
#include <cstring>
// win32
#include <window/window.hpp>

namespace impl {

///
/// 窗口搜索后的结果
/// ---------------------------------------------------------
/// 把标题和对应的handle关联起来
///
struct FindWindowResult {
    PCHAR title;
    HWND  handle;
};

///
/// find_window的回调函数
/// ---------------------------------------------------------
/// 在找到第一个匹配title的窗口后就停止
///
static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
{
    CHAR title[1024];
    int  length = GetWindowTextA(hwnd, title, sizeof(title));

    // 有的无窗口应用还是会有hwnd，在这里过滤掉
    // ---------------------------------------------------------
    if (!IsWindowVisible(hwnd) ||
        length == 0 ||
        std::strcmp(title, "Program Manager") == 0) {
        return TRUE;
    }

    // 找到对应的窗口就立即返回，否则继续
    // ---------------------------------------------------------
    auto result = reinterpret_cast<FindWindowResult*>(lParam);
    if (std::strncmp(title, result->title, std::strlen(result->title)) == 0) {
        result->handle = hwnd;
        return FALSE;
    }

    return TRUE;
}

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

namespace autozhuxian {

///
/// Window的方法
/// ---------------------------------------------------------
/// 整个窗口内容都作为ROI
///
RegionOfInterest Window::roi()
{
    return RegionOfInterest{
        0,
        0,
        rect_width(m_window_info.rcClient),
        rect_height(m_window_info.rcClient),
    };
}

///
/// Window的方法
/// ---------------------------------------------------------
/// 上边框 + 标题栏的高度，转换绝对坐标的时候要加上去
///
int Window::title_and_border_height()
{
    int win_height = rect_height(m_window_info.rcWindow);
    int cli_height = rect_height(m_window_info.rcClient);
    // 窗口高度 - 画布高度 = 标题栏 + 上下边框的高度
    // 所以需要再剪掉一次边框的高度，才等于上边框 + 标题栏的高度
    return win_height - cli_height - m_window_info.cyWindowBorders;
}

///
/// autozhuxian下的函数
/// ---------------------------------------------------------
/// 根据标题找到对应的窗口
///
std::optional<Window> find_window(PCHAR title)
{
    impl::FindWindowResult result{title, nullptr};

    EnumWindows(impl::EnumWindowCb, reinterpret_cast<LPARAM>(&result));

    if (result.handle) {
        return std::optional{Window{title, result.handle}};
    } else {
        return std::nullopt;
    }
}

};  // namespace autozhuxian