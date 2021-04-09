#include <cstring>
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
    if (std::strcmp(title, result->title) == 0) {
        result->handle = hwnd;
        return FALSE;
    }

    return TRUE;
}

};  // namespace impl

namespace autozhuxian {

std::optional<Window> find_window(PCHAR title)
{
    impl::FindWindowResult result{title, nullptr};

    EnumWindows(impl::EnumWindowCb, reinterpret_cast<LPARAM>(&result));

    return result.handle ? Window{title, result.handle} : {};
}

};  // namespace autozhuxian