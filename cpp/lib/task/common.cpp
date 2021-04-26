// std
#include <functional>
#include <thread>
#include <chrono>
#include <map>
#include <ctime>
#include <cmath>
// project
#include <task/common.hpp>
#include <task/command.hpp>
#include <control/control.hpp>
#include <macro/assert.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\common\\"
#define PATH(p) BASE p

#define CLICKUI(p) "点击界面：" p

using namespace std::chrono_literals;

namespace impl {

///
/// 输入一串按键
/// ---------------------------------------------------------
///
///
static void input(std::vector<DWORD>& keycodes)
{
    for (auto keycode : keycodes) {
        autozhuxian::press(keycode);
        std::this_thread::sleep_for(200ms);
    }
}

///
/// 获取数字num的长度
/// ---------------------------------------------------------
///
///
static int get_num_len(int num)
{
    int tens = 0;
    while (num >= 10) {
        tens++;
        num /= 10;
    }
    return tens;
}

///
/// 输入一个数字
/// ---------------------------------------------------------
/// 将数字转换成对应的按键
///
static void input(int num)
{
    std::vector<DWORD> keycodes;

    for (int len = get_num_len(num); len >= 0; --len) {
        int num_digits = std::pow(10, len);
        int digit = num / num_digits;
        keycodes.push_back(digit + 0x30);
        num -= digit * num_digits;
    }

    input(keycodes);
}

///
/// 输入一个坐标
/// ---------------------------------------------------------
/// 将坐标转换成对应的按键
///
static void input(cv::Point location)
{
    input(location.x);
    autozhuxian::press(VK_OEM_COMMA);
    std::this_thread::sleep_for(200ms);
    input(location.y);
}

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
    std::this_thread::sleep_for(300ms);
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
static UIPos get_pos(autozhuxian::common_task::UIType type)
{
    using namespace autozhuxian::common_task;
    static std::map<UIType, UIPos> infos{
        {UIType::Reward, {CLICKUI("奖励"), {1418, 115}}},
        {UIType::Update, {CLICKUI("更新"), {1479, 203}}},
        {UIType::PVP, {CLICKUI("竞技"), {1490, 988}}},
        {UIType::Character, {CLICKUI("角色"), {1270, 988}}},
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
static std::unique_ptr<autozhuxian::Command> get_cmd(autozhuxian::common_task::UIType type)
{
    auto info = impl::get_pos(type);
    return std::make_unique<autozhuxian::ClickByPositionCmd>(info.name,
                                                             info.pos,
                                                             1'000);
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
        result.push_back(ZXWindow{"诛仙3", hwnd});
    }

    return TRUE;
}

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

namespace autozhuxian::common_task {

///
/// 常用功能
/// ---------------------------------------------------------
/// 移动到某一坐标
///
void move_to(Window& win, cv::Point location)
{
    activate(win);

    ClickByImageCmd cmd{"点击输入框",
                        PATH("map_input.png"),
                        cv::Point{0, 32},  // TODO move right
                        500};
    do {
        press_esc();
        press(0x4D);  // M
        std::this_thread::sleep_for(500ms);
    } while (!cmd.execute(win));

    // TODO delete last input location

    impl::input(location);

    // TODO click move and delete
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 获取今天是星期几
///
int get_weekday()
{
    auto        now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm*    tm = std::localtime(&t);
    int         weekday = tm->tm_wday;
    return weekday == 0 ? 7 : weekday;
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 狂点ESC，最大程度关闭一些窗口或对话框
///
void press_esc()
{
    impl::repeat(5, impl::press_esc);
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 关闭所有的弹出框
/// 用于登录后，关闭领取奖励和更新说明的弹框
///
void close_all_ui(Window& win)
{
    press_esc();
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
    activate(win);
    open_ui(win, UIType::Character);

    for (int i = 0; i < static_cast<int>(RoleType::TOTAL); ++i) {
        impl::RoleInfo  info = impl::get_role_name(static_cast<RoleType>(i));
        ConfirmImageCmd cmd{"寻找窗口对应的角色", info.path};
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
        auto info = find_role(win);
        win.bind_role(info.name, info.type);
    }
    return wins;
}

///
/// 常用功能
/// ---------------------------------------------------------
/// 把窗口调到前台
///
void activate(Window& win)
{
    SetForegroundWindow(win.handle());
    std::this_thread::sleep_for(500ms);  // 确保真的调到了前台
}

};  // namespace autozhuxian::common_task