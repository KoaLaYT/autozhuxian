#include <gtest/gtest.h>

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\tmp\\"
#define PATH(p) BASE p

static void click()
{
    using namespace std::chrono_literals;

    INPUT Input{};

    Input.type = INPUT_MOUSE;
    Input.mi.dx = 9000;  // desired X coordinate
    Input.mi.dy = 9000;  // desired Y coordinate
    Input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    SendInput(1, &Input, sizeof(INPUT));

    std::this_thread::sleep_for(1000ms);

    Input.mi.dx = 0;
    Input.mi.dy = 0;
    for (int i = 0; i < 10; ++i) {
        Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &Input, sizeof(INPUT));
        Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &Input, sizeof(INPUT));
    }
}

TEST(windows_api, wchar)
{
}