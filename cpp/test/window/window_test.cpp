#include <window/window.hpp>
#include <gtest/gtest.h>

TEST(window, find_window)
{
    HWND wanmei_platform = autozhuxian::find_window("完美游戏平台");
    ASSERT_TRUE(wanmei_platform);
}