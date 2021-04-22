#ifndef KOALAYT_TASK_HPP_20210416
#define KOALAYT_TASK_HPP_20210416

// std
#include <vector>
// project
#include <task/command.hpp>
#include <window/window.hpp>

using namespace autozhuxian;

///
/// 登录游戏
/// ---------------------------------------------------------
/// 在游戏打开后，选择服务器与人物登录
///
class LoginTask {
public:
    LoginTask();

    // ---------------------------------------------------------
    // 执行该任务
    //
    void run(Window& win);

private:
    std::vector<std::unique_ptr<Command>> m_cmds;
};

///
/// 启动游戏
/// ---------------------------------------------------------
/// 从启动器启动诛仙，包括处理游戏更新
///
class LaunchTask : public Logger<LaunchTask> {
public:
    LaunchTask() { init(); }

    // ---------------------------------------------------------
    // 执行该任务
    //
    void run();

private:
    Window m_platform;  // 完美游戏平台的窗口

    // ---------------------------------------------------------
    // 打开完美游戏平台
    //
    void open_platform();

    // ---------------------------------------------------------
    // 等待游戏更新完成
    //
    void wait_updated(Window& win);

    // ---------------------------------------------------------
    // 初始化窗口
    //
    void init();
};

#endif