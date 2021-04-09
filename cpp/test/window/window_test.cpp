#include <gtest/gtest.h>

#include <window/window.hpp>

TEST(window, find_window)
{
    auto wanmei_platform = autozhuxian::find_window("完美游戏平台");
    ASSERT_TRUE(wanmei_platform);
    ASSERT_TRUE(wanmei_platform->handle());
}