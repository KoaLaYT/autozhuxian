// std
#include <functional>
#include <thread>
#include <chrono>
#include <map>
// project
#include <util/common_task.hpp>
#include <control/control.hpp>
#include <command/command.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\common\\"
#define PATH(p) BASE p

namespace impl {

///
/// 反复运行某个lambda
/// ---------------------------------------------------------
///
///
static void repeat(int n, std::function<void()> func)
{
    while (n-- > 0)
        func();
}

struct UIPos {
    const char* name;
    cv::Point   pos;
};

struct RoleInfo {
    const char* path;
    const char* name;
};

///
/// 一些常用UI的坐标位置
/// ---------------------------------------------------------
///
///
static UIPos get_ui_pos(autozhuxian::util::UIType type)
{
    using namespace autozhuxian::util;
    static std::map<UIType, UIPos> infos{
        {UIType::Reward, {"打开奖励界面", {1418, 115}}},
        {UIType::Update, {"打开更新说明", {1479, 203}}},
        {UIType::PVP, {"打开竞技", {1490, 988}}},
        {UIType::Character, {"打开角色", {1270, 988}}},
    };
    // TODO check
    return infos[type];
}

///
/// 常玩的几个角色，对应名字的文件路径
/// ---------------------------------------------------------
/// 即根据名字来找到对应的窗口
///
static RoleInfo get_role_name(autozhuxian::util::RoleType type)
{
    using namespace autozhuxian::util;
    static std::map<RoleType, RoleInfo> infos{
        {RoleType::HeHuan, {PATH("hehuan.png"), "合欢"}},
        {RoleType::PoJun, {PATH("pojun.png"), "破军"}},
    };
    // TODO check
    return infos[type];
}

};  // namespace impl

namespace autozhuxian::util {

using namespace std::chrono_literals;

///
/// 常用功能
/// ---------------------------------------------------------
/// 关闭所有的弹出框
///
void close_all_ui(Window& win)
{
    auto press_esc = []() {
        press(VK_ESCAPE);
        std::this_thread::sleep_for(100ms);
    };

    impl::repeat(5, press_esc);
    open_ui(win, UIType::Reward);
    press_esc();
    open_ui(win, UIType::Update);
    press_esc();
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 打开常用的UI界面
///
void open_ui(Window& win, UIType type)
{
    auto               info = impl::get_ui_pos(type);
    ClickByPositionCmd cmd{
        info.name,
        info.pos,
        300,
    };
    // 为了防止打开前，该ui已经打开，
    // 这里处理的方式是先点击打开按钮，然后esc取消，这时确保了ui一定处于关闭状态
    // 然后再打开即可
    // ---------------------------------------------------------
    cmd.execute(win);
    press(VK_ESCAPE);
    std::this_thread::sleep_for(100ms);
    cmd.execute(win);
}

///
/// 找到窗口对应的角色
/// ---------------------------------------------------------
/// 主要是为了之后的日志功能，输出时可以知道是哪个职业
///
RoleInfo find_role(Window& win)
{
    open_ui(win, UIType::Character);
    for (int i = 0; i < static_cast<int>(RoleType::TOTAL); ++i) {
        impl::RoleInfo  info = impl::get_role_name(static_cast<RoleType>(i));
        ConfirmImageCmd cmd{
            "寻找窗口对应的角色",
            RegionOfInterest::whole,
            info.path,
        };
        if (cmd.execute(win)) {
            return RoleInfo{static_cast<RoleType>(i), info.name};
        }
    }
    // TODO what about not find?
}

};  // namespace autozhuxian::util