// std
#include <cstdio>
#include <cstring>
// win32
#include <window/window.hpp>

namespace impl {

struct FindWindowResult {
    PCHAR title;
    HWND handle;
};

/**
 * @brief find_window的回调函数，在找到第一个匹配title的窗口后就停止
 * 
 * @param hwnd 当前遍历到的窗口
 * @param lParam EnumWindows传入的参数
 * @return TRUE 继续下一个窗口
 *         FALSE 停止
 */
static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
{
    CHAR title[1024];
    int length = GetWindowTextA(hwnd, title, sizeof(title));

    // 有的无窗口应用还是会有hwnd，在这里过滤掉
    if (!IsWindowVisible(hwnd) ||
        length == 0 ||
        std::strcmp(title, "Program Manager") == 0) {
        return TRUE;
    }

    auto result = reinterpret_cast<FindWindowResult*>(lParam);
    if (std::strncmp(title, result->title, std::strlen(result->title)) == 0) {
        result->handle = hwnd;
        return FALSE;
    }

    return TRUE;
}

};  // namespace impl

namespace autozhuxian {

RegionOfInterest Window::roi()
{
    return RegionOfInterest{
        0,
        0,
        rect_width(m_window_info.rcWindow),
        rect_height(m_window_info.rcWindow),
    };
}

int Window::title_and_border_height()
{
    int win_height = rect_height(m_window_info.rcWindow);
    int cli_height = rect_height(m_window_info.rcClient);
    // 窗口高度 - 画布高度 = 标题栏 + 上下边框的高度
    // 所以需要再剪掉一次边框的高度，才等于上边框 + 标题栏的高度
    return win_height - cli_height - m_window_info.cyWindowBorders;
}

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