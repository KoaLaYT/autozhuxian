// std
#include <functional>
#include <thread>
#include <chrono>
#include <map>
// project
#include <util/common_task.hpp>
#include <control/control.hpp>
#include <command/command.hpp>
#include <macro/assert.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\common\\"
#define PATH(p) BASE p

namespace impl {

using namespace std::chrono_literals;

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

///
/// 按ESC后等待一小段时间
/// ---------------------------------------------------------
/// 通过ESC来确保关闭一些窗口
///
static void press_esc()
{
    autozhuxian::press(VK_ESCAPE);
    std::this_thread::sleep_for(100ms);
}

///
/// 常用UI的名称及其对应的坐标位置
/// ---------------------------------------------------------
///
///
struct UIPos {
    const char* name;
    cv::Point   pos;
};

///
/// 常玩的角色名称及其图片的路径
/// ---------------------------------------------------------
///
///
struct RoleInfo {
    const char* path;
    const char* name;
};

///
/// 一些常用UI的坐标位置
/// ---------------------------------------------------------
///
///
static UIPos get_pos(autozhuxian::util::UIType type)
{
    using namespace autozhuxian::util;
    static std::map<UIType, UIPos> infos{
        {UIType::Reward, {"打开奖励界面", {1418, 115}}},
        {UIType::Update, {"打开更新说明", {1479, 203}}},
        {UIType::PVP, {"打开竞技", {1490, 988}}},
        {UIType::Character, {"打开角色", {1270, 988}}},
    };
    auto found = infos.find(type);
    AUTOZHUXIAN_ASSERT(found != infos.end(), "未定义的类型");
    return found->second;
}

///
/// 获取点击某个UI按钮的命令
/// ---------------------------------------------------------
///
///
static std::unique_ptr<autozhuxian::Command> get_cmd(autozhuxian::util::UIType type)
{
    auto info = impl::get_pos(type);
    return std::make_unique<autozhuxian::ClickByPositionCmd>(info.name,
                                                             info.pos,
                                                             300);
}

///
/// 常玩的几个角色，对应名字的文件路径
/// ---------------------------------------------------------
/// 即根据名字来找到对应的窗口
///
static RoleInfo get_role_name(autozhuxian::RoleType type)
{
    using namespace autozhuxian;
    static std::map<RoleType, RoleInfo> infos{
        {RoleType::HeHuan, {PATH("hehuan.png"), "合欢"}},
        {RoleType::PoJun, {PATH("pojun.png"), "破军"}},
    };
    auto found = infos.find(type);
    AUTOZHUXIAN_ASSERT(found != infos.end(), "未定义的类型");
    return found->second;
}

///
/// 搜索窗口的回调函数
/// ---------------------------------------------------------
///
///
static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
{
    using ZXWindow = autozhuxian::Window;
    CHAR title[1024];
    int  length = GetWindowTextA(hwnd, title, sizeof(title));

    // 有的无窗口应用还是会有hwnd，在这里过滤掉
    if (!IsWindowVisible(hwnd) ||
        length == 0 ||
        std::strcmp(title, "Program Manager") == 0) {
        return TRUE;
    }

    std::vector<ZXWindow>& result = *reinterpret_cast<std::vector<ZXWindow>*>(lParam);
    if (std::strcmp(title, "诛仙3") == 0) {
        std::printf("找到一个诛仙窗口\n");
        result.push_back(ZXWindow{"诛仙3", hwnd});
    }

    return TRUE;
}

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

namespace autozhuxian::util {

///
/// 常用功能
/// ---------------------------------------------------------
/// 关闭所有的弹出框
/// 用于登录后，关闭领取奖励和更新说明的弹框
///
void close_all_ui(Window& win)
{
    impl::repeat(5, impl::press_esc);
    open_ui(win, UIType::Reward);
    impl::press_esc();
    open_ui(win, UIType::Update);
    impl::press_esc();
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 打开常用的UI界面
///
void open_ui(Window& win, UIType type)
{
    auto cmd = impl::get_cmd(type);
    // 为了防止打开前，该ui已经打开，
    // 这里处理的方式是先点击打开按钮，然后esc取消，这时确保了ui一定处于关闭状态
    // 然后再打开即可
    // ---------------------------------------------------------
    cmd->execute(win);
    impl::press_esc();
    cmd->execute(win);
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 关闭常见的UI界面
///
void close_ui(Window& win, UIType type)
{
    auto cmd = impl::get_cmd(type);
    // 这样无论该UI是否已经打开
    // 通过一次点开关闭的操作后，一定处于关闭状态
    // ---------------------------------------------------------
    cmd->execute(win);
    impl::press_esc();
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 找到窗口对应的角色
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
            close_ui(win, UIType::Character);
            return RoleInfo{static_cast<RoleType>(i), info.name};
        }
    }

    close_ui(win, UIType::Character);
    return RoleInfo{RoleType::Unknown, "未知角色"};
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 找到所有的诛仙窗口
///
std::vector<Window> find_all_zx_wins()
{
    std::vector<Window> wins;
    EnumWindows(impl::EnumWindowCb, reinterpret_cast<LPARAM>(&wins));
    for (auto& win : wins) {
        SetForegroundWindow(win.handle());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto info = find_role(win);
        win.bind_role(info.name, info.type);
    }
    return wins;
}

};  // namespace autozhuxian::util