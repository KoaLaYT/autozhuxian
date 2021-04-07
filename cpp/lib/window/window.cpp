#include <window/window.hpp>
#include <cstring>
#include <cstdio>

namespace impl {

struct FindWindowResult {
    PCHAR title;
    HWND handle;
};

static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
{
    CHAR title[1024];
    int length = GetWindowTextA(hwnd, title, sizeof(title));

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

HWND find_window(PCHAR title)
{
    impl::FindWindowResult result{title, nullptr};

    EnumWindows(impl::EnumWindowCb, reinterpret_cast<LPARAM>(&result));

    return result.handle;
}

};  // namespace autozhuxian