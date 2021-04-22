#ifndef KOALAYT_COMMON_HPP_20210419
#define KOALAYT_COMMON_HPP_20210419

#include <vector>
// project
#include <window/window.hpp>

namespace autozhuxian::common_task {

///
/// 常用的ui界面类型
/// ---------------------------------------------------------
///
///
enum class UIType {
    Reward,     // 奖励
    Update,     // 更新提示
    PVP,        // 竞技
    Character,  // 角色
};

///
/// 角色的信息
/// ---------------------------------------------------------
/// 包括类型及对应的名称
///
struct RoleInfo {
    autozhuxian::RoleType type;
    const char*           name;
};

///
/// 关闭所有的弹出框
/// ---------------------------------------------------------
/// 一般用于登陆后，关闭奖励领取和更新提示
//? 弹出的任务活动对话框如何关闭
///
void close_all_ui(Window& win);

///
/// 打开常见的UI界面
/// ---------------------------------------------------------
/// 比如奖励领取、竞技等
///
void open_ui(Window& win, UIType type);

///
/// 关闭常见的UI界面
/// ---------------------------------------------------------
/// 比如奖励领取、竞技等
///
void close_ui(Window& win, UIType type);

///
/// 找到窗口对应的角色
/// ---------------------------------------------------------
/// 主要是为了之后的日志功能，输出时可以知道是哪个职业
///
RoleInfo find_role(Window& win);

///
/// 找到所有的诛仙窗口
/// ---------------------------------------------------------
/// 并且会将窗口对应的角色关联起来
///
std::vector<Window> find_all_zx_wins();

///
/// 把窗口调到前台
/// ---------------------------------------------------------
/// 只有在前台的窗口，UI才会更新，
/// 鼠标、键盘的指令才能正常作用
///
void activate(Window& win);

};  // namespace autozhuxian::common_task

#endif