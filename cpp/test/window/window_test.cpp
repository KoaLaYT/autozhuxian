// std
#include <cstdio>
#include <thread>
#include <chrono>
// third party
#include <gtest/gtest.h>
// project
#include <window/window.hpp>
#include <imagesearch/screenshot.hpp>
#include <command/command.hpp>
#include <control/control.hpp>
#include <util/common_task.hpp>

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
    auto wanmei_platform = autozhuxian::find_window("诛仙3");
    ASSERT_TRUE(wanmei_platform);
    ASSERT_TRUE(wanmei_platform->handle());

    Size win_size = get_rect_size(wanmei_platform->window_info().rcClient);
    win_size.print();

    cv::Mat screenshot = autozhuxian::screenshot_region(wanmei_platform->handle(),
                                                        autozhuxian::RegionOfInterest{0, 0, win_size.width, win_size.height});
    cv::imwrite(name, screenshot);
    autozhuxian::click();
}

// 领奖励的流程
class RewardTask {
public:
    void run(autozhuxian::Window& win)
    {
        SetForegroundWindow(win.handle());
        close_all_ui(win);
        open_reward_ui(win);
        signup(win);
        levelup(win);
        close_all_ui(win);
    }

private:
    // [1] 首先点击右上角的按钮，然后esc，可以确保界面处于关闭状态
    //     抹平了更新与非更新、第一次登陆与非第一次登录的差异 // TODO 更新活动的弹框如何关闭？
    void close_all_ui(autozhuxian::Window& win)
    {
        {
            autozhuxian::ClickByPositionCmd cmd{
                "打开奖励界面",
                cv::Point{1418, 115},
                500,
            };
            cmd.execute(win);
        }

        {
            for (int i = 0; i < 10; ++i) {
                autozhuxian::press(VK_ESCAPE);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        {
            autozhuxian::ClickByPositionCmd cmd{
                "打开更新说明",
                cv::Point{1479, 203},
                500,
            };
            cmd.execute(win);
        }

        {
            for (int i = 0; i < 10; ++i) {
                autozhuxian::press(VK_ESCAPE);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
    // [2] 点击奖励，确认界面出现
    void open_reward_ui(autozhuxian::Window& win)
    {
        bool is_open = false;
        while (!is_open) {
            autozhuxian::ClickByPositionCmd cmd1{
                "打开奖励界面",
                cv::Point{1418, 115},
                500,
            };
            cmd1.execute(win);

            autozhuxian::ConfirmImageCmd cmd2{
                "确认奖励界面已打开",
                autozhuxian::RegionOfInterest::whole,
                autozhuxian::ImageSearchTargets{
                    {PATH("reward\\active.png")},
                    {PATH("reward\\levelup.png")},
                    {PATH("reward\\signup.png")},
                },
            };
            is_open = cmd2.execute(win);
        }
    }
    // [3] 签到
    void signup(autozhuxian::Window& win)
    {
        std::vector<std::unique_ptr<autozhuxian::Command>> cmds;
        cmds.emplace_back(std::make_unique<autozhuxian::ClickByImageCmd>("点击签到奖励",
                                                                         autozhuxian::RegionOfInterest::whole,
                                                                         autozhuxian::ImageSearchTargets{{PATH("reward\\signup.png")},
                                                                                                         {PATH("reward\\signup_selected.png")}},

                                                                         200));
        cmds.emplace_back(std::make_unique<autozhuxian::ClickByImageCmd>("领取签到奖励",
                                                                         autozhuxian::RegionOfInterest::whole,
                                                                         PATH("reward\\signup_confirm.png"),
                                                                         200));
        for (auto& cmd : cmds) {
            cmd->execute(win);
        }
    }
    // [4] 升级奖励
    void levelup(autozhuxian::Window& win)
    {
        std::vector<std::unique_ptr<autozhuxian::Command>> cmds;
        cmds.emplace_back(std::make_unique<autozhuxian::ClickByImageCmd>("点击升级奖励",
                                                                         autozhuxian::RegionOfInterest::whole,
                                                                         autozhuxian::ImageSearchTargets{{PATH("reward\\levelup.png")},
                                                                                                         {PATH("reward\\levelup_selected.png")}},

                                                                         200));
        cmds.emplace_back(std::make_unique<autozhuxian::ClickByImageCmd>("领取升级奖励",
                                                                         autozhuxian::RegionOfInterest::whole,
                                                                         PATH("reward\\levelup_confirm.png"),
                                                                         200));
        for (auto& cmd : cmds) {
            cmd->execute(win);
        }
    }
};

TEST(window, close_window)
{
    // screenshot_zx(PATH("tmp\\screen.png"));

    auto zx = autozhuxian::find_window("诛仙3");
    ASSERT_TRUE(zx);
    SetForegroundWindow(zx->handle());

    // autozhuxian::util::close_all_ui(zx.value());
    // autozhuxian::util::open_ui(zx.value(), autozhuxian::util::UIType::Character);
    auto info = autozhuxian::util::find_role(zx.value());
    std::printf("这个窗口是：%s\n", info.name);

    // autozhuxian::ClickByImageCmd cmd{
    //     "关闭弹框",
    //     autozhuxian::RegionOfInterest::whole,
    //     PATH("common\\close_btn.png"),
    //     0,
    // };
    // cmd.execute(zx.value());

    // RewardTask task;
    // task.run(zx.value());
}