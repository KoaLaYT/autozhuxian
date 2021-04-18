#include <gtest/gtest.h>

#include <cstdio>

#include <window/window.hpp>
#include <imagesearch/screenshot.hpp>
#include <command/command.hpp>
#include <control/control.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\"
#define PATH(p) BASE p

struct Size {
    int width;
    int height;

    void print()
    {
        std::printf("size: %d x %d\n", width, height);
    }
};

Size get_rect_size(RECT& rect)
{
    return Size{rect.right - rect.left,
                rect.bottom - rect.top};
}

void screenshot_zx(const char* name)
{
    auto wanmei_platform = autozhuxian::find_window("完美");
    ASSERT_TRUE(wanmei_platform);
    ASSERT_TRUE(wanmei_platform->handle());

    Size win_size = get_rect_size(wanmei_platform->window_info().rcClient);
    win_size.print();

    cv::Mat screenshot = autozhuxian::screenshot_region(wanmei_platform->handle(),
                                                        autozhuxian::RegionOfInterest{0, 0, win_size.width, win_size.height});
    cv::imwrite(name, screenshot);
    autozhuxian::click();
}

TEST(window, close_window)
{
    screenshot_zx(PATH("tmp\\screen.png"));

    // auto zx = autozhuxian::find_window("诛仙3");
    // ASSERT_TRUE(zx);
    // SetForegroundWindow(zx->handle());

    // autozhuxian::ClickByImageCmd cmd{
    //     "关闭弹框",
    //     autozhuxian::RegionOfInterest::whole,
    //     PATH("common\\close_btn.png"),
    //     0,
    // };
    // cmd.execute(zx.value());

    // {
    //     autozhuxian::ClickByPositionCmd cmd{
    //         "打开奖励界面",
    //         cv::Point{1418, 115},
    //         500,
    //     };
    //     cmd.execute(zx.value());
    // }

    // std::vector<autozhuxian::Command*> cmds{
    //     new autozhuxian::ConfirmImageCmd{
    //         "确认奖励界面已打开",
    //         autozhuxian::RegionOfInterest::whole,
    //         autozhuxian::ImageSearchTargets{
    //             {PATH("reward\\active.png")},
    //             {PATH("reward\\levelup.png")},
    //             {PATH("reward\\signup.png")},
    //         },
    //     },
    //     new autozhuxian::ClickByImageCmd{
    //         "点击签到奖励",
    //         autozhuxian::RegionOfInterest::whole,
    //         autozhuxian::ImageSearchTargets{{PATH("reward\\signup.png")},
    //                                         {PATH("reward\\signup_selected.png")}},

    //         200,
    //     },
    //     new autozhuxian::ClickByImageCmd{
    //         "领取签到奖励",
    //         autozhuxian::RegionOfInterest::whole,
    //         PATH("reward\\signup_confirm.png"),
    //         200,
    //     },
    //     new autozhuxian::ClickByImageCmd{
    //         "点击升级奖励",
    //         autozhuxian::RegionOfInterest::whole,
    //         autozhuxian::ImageSearchTargets{{PATH("reward\\levelup.png")},
    //                                         {PATH("reward\\levelup_selected.png")}},

    //         200,
    //     },
    //     new autozhuxian::ClickByImageCmd{
    //         "领取升级奖励",
    //         autozhuxian::RegionOfInterest::whole,
    //         PATH("reward\\levelup_confirm.png"),
    //         200,
    //     },
    // };

    // for (auto& cmd : cmds) {
    //     cmd->execute(zx.value());
    // }
}