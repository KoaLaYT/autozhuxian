// std
#include <thread>
#include <chrono>
#include <cstdio>
// project
#include <control/control.hpp>
#include "task.hpp"
#include "macro.hpp"

using namespace std::chrono_literals;

///
/// 登录游戏
/// ---------------------------------------------------------
/// Constructor
///
LoginTask::LoginTask()
{
    m_cmds.emplace_back(std::make_unique<ClickByImageCmd>(
        "寻找服务器承天竹影",
        RegionOfInterest{400, 0, 800, 1024},
        PATH("server.png"),
        0));

    m_cmds.emplace_back(std::make_unique<ClickByImageCmd>(
        "选择服务器",
        RegionOfInterest{400, 0, 800, 1024},
        PATH("confirm.png"),
        2'000));

    m_cmds.emplace_back(std::make_unique<ClickByPositionCmd>(
        "进入游戏",
        cv::Point{800, 962},
        200));
}

///
/// 登录游戏
/// ---------------------------------------------------------
/// 运行登录游戏的命令流
///
void LoginTask::run(Window& win)
{
    // 切换窗口，等待界面刷新
    // ---------------------------------------------------------
    SetForegroundWindow(win.handle());
    std::this_thread::sleep_for(1000ms);

    for (auto& cmd : m_cmds) {
        cmd->execute(win);
    }
}

///
/// 启动游戏
/// ---------------------------------------------------------
/// 初始化窗口
///
void LaunchTask::init()
{
    info("{:-^30}", "");
    info("{}", "开始启动游戏");
    info("{:-^30}", "");
    while (true) {
        auto platform = find_window("完美游戏平台");
        if (!platform) {
            info("完美游戏平台未启动，尝试打开");
            open_platform();
        } else {
            info("完美游戏平台已打开");
            m_platform = platform.value();
            break;
        }
    }
}

///
/// 启动游戏
/// ---------------------------------------------------------
/// 开始更新游戏，或者多开启动诛仙3
///
void LaunchTask::run()
{
    SetForegroundWindow(m_platform.handle());

    // TODO update check

    wait_updated(m_platform);

    // 点击多开按钮，会弹出一个新窗口
    // ---------------------------------------------------------
    {
        ClickByImageCmd cmd{
            "点击游戏多开",
            PATH("multiopen.png"),
            1000,
        };
        cmd.execute(m_platform);
    }

    // 在这个新窗口中点击开始游戏
    // ---------------------------------------------------------
    auto sub_win = find_window("游戏多开");
    if (!sub_win) {
        throw std::runtime_error{"打开多开窗口失败"};
    }
    ClickByImageCmd cmd{
        "点击开始多开游戏",
        RegionOfInterest{0, 0, 621, 334},
        PATH("multistart.png"),
        60'000,
    };
    cmd.execute(sub_win.value());
}

///
/// 启动游戏
/// ---------------------------------------------------------
/// 等待游戏更新完成
///
void LaunchTask::wait_updated(Window& win)
{
    // 循环判断游戏是否更新完成
    // ---------------------------------------------------------
    ConfirmImageCmd cmd{"检查游戏更新是否完成", PATH("start_game.png")};
    while (!cmd.execute(win)) {
        info("游戏正在更新");
        // 没更新好就等5秒，再检查一遍
        std::this_thread::sleep_for(5'000ms);
    }
    info("游戏已更新");
}

///
/// 启动游戏
/// ---------------------------------------------------------
/// 打开完美游戏平台
///
void LaunchTask::open_platform()
{
    Window          desktop{"桌面", GetDesktopWindow()};
    ClickByImageCmd cmd{
        "点击开始屏幕中的完美游戏平台icon",
        PATH("wanmei_platform.png"),
        30'000,
    };

    // 点击左边的win键，打开开始屏幕，并且等待它的动画结束
    // ---------------------------------------------------------
    press(VK_LWIN);
    std::this_thread::sleep_for(2'000ms);

    // 执行操作
    // ---------------------------------------------------------
    if (!cmd.execute(desktop)) {
        throw std::runtime_error{"启动诛仙3失败"};
    }
}