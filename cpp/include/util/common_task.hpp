#ifndef KOALAYT_COMMON_TASK_HPP_20210419
#define KOALAYT_COMMON_TASK_HPP_20210419

// project
#include <window/window.hpp>

namespace autozhuxian::util {

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
/// 常玩的角色
/// ---------------------------------------------------------
///
///
enum class RoleType {
    HeHuan,  // 合欢
    PoJun,   // 破军
    TOTAL,
    Unknown,  // 未知，用于搜索失败的情况
};

///
/// 角色的信息
/// ---------------------------------------------------------
/// 包括类型及对应的名称
///
struct RoleInfo {
    autozhuxian::util::RoleType type;
    const char*                 name;
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

};  // namespace autozhuxian::util

#endif