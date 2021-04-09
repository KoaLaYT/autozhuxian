#include <gtest/gtest.h>

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

#include <cstdio>

TEST(windows_api, wchar)
{
    HWND desktop = GetDesktopWindow();
    RECT rect;
    GetClientRect(desktop, &rect);
    std::printf("%d X %d\n", rect.right - rect.left, rect.bottom - rect.top);
}