#include <cstdio>
#include <thread>
#include <vector>

#include <window/window.hpp>
#include <control/control.hpp>
#include <imagesearch/screenshot.hpp>
#include <imagesearch/match.hpp>
#include <task/task.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\login\\"
#define PATH(p) BASE p

using namespace std::chrono_literals;
using ZXWindow = autozhuxian::Window;

// TODO remove duplicate logic
static void open_platform();
static void start_game(ZXWindow& win);
static void confirm_updated(ZXWindow& win);

struct WindowManager {
    std::vector<ZXWindow> windows;

    static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
    {
        CHAR title[1024];
        int length = GetWindowTextA(hwnd, title, sizeof(title));

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

    WindowManager()
    {
        EnumWindows(EnumWindowCb, reinterpret_cast<LPARAM>(&windows));
    }
};

static std::vector<autozhuxian::Command*> login_task()
{
    using namespace autozhuxian;
    return std::vector<Command*>{
        new ClickByImageCmd{
            "寻找服务器承天竹影",
            RegionOfInterest{400, 0, 800, 1024},
            PATH("server.png"),
            0,
        },
        new ClickByImageCmd{
            "选择服务器",
            RegionOfInterest{400, 0, 800, 1024},
            PATH("confirm.png"),
            2'000,
        },
        new ClickByPositionCmd{
            "进入游戏",
            cv::Point{800, 962},
            10'000,
        },
    };
}

int main()
{
    char* platform_title = "完美游戏平台";
    char* platform_subwin = "游戏多开";
    auto platform = autozhuxian::find_window(platform_title);
    if (!platform) {
        std::printf("未找到窗口，尝试打开窗口\n");
        open_platform();
        return main();
    }

    std::printf("完美游戏平台已打开\n");
    start_game(platform.value());

    std::printf("诛仙已开始\n");
    WindowManager wm;

    for (auto& win : wm.windows) {
        // 切换窗口，等待界面刷新
        SetForegroundWindow(win.handle());
        std::this_thread::sleep_for(1000ms);

        for (auto& cmd : login_task()) {
            cmd->execute(win);
        }
    }

    return 0;
}

static void open_platform()
{
    autozhuxian::press(VK_LWIN);
    // waiting for the window shows
    std::this_thread::sleep_for(2000ms);
    autozhuxian::move(0, 0);

    HWND desktop_hwnd = GetDesktopWindow();
    autozhuxian::Window desktop{"桌面", desktop_hwnd};
    cv::Mat source = autozhuxian::screenshot_region(desktop_hwnd, desktop.roi());
    cv::Mat platform_icon = cv::imread(PATH("wanmei_platform.png"), cv::IMREAD_UNCHANGED);

    autozhuxian::Matcher matcher{source};
    auto location = matcher.search(platform_icon);
    if (!location) {
        throw std::runtime_error{"未找到完美游戏平台icon"};
    }

    autozhuxian::click(location->x + platform_icon.rows / 2,
                       location->y + platform_icon.cols / 2);

    // waiting for the window shows
    std::this_thread::sleep_for(10'000ms);
}

static void start_game(ZXWindow& win)
{
    SetForegroundWindow(win.handle());
    // TODO check if need update

    confirm_updated(win);

    {
        autozhuxian::ClickByImageCmd cmd{
            "点击游戏多开",
            autozhuxian::RegionOfInterest::whole,
            PATH("multiopen.png"),
            1000,
        };
        cmd.execute(win);
    }

    auto sub_win = autozhuxian::find_window("游戏多开");
    if (!sub_win) {
        throw std::runtime_error{"打开多开窗口失败"};
    }
    autozhuxian::ClickByImageCmd cmd{
        "点击开始多开游戏",
        autozhuxian::RegionOfInterest::whole,
        PATH("multistart.png"),
        60'000,
    };
    cmd.execute(sub_win.value());
}

static void confirm_updated(autozhuxian::Window& win)
{
    cv::Mat target = cv::imread(PATH("start_game.png"), cv::IMREAD_UNCHANGED);
    std::optional<cv::Point> position = std::nullopt;
    while (true) {
        auto screenshot = autozhuxian::screenshot_region(win.handle(), win.roi());
        autozhuxian::Matcher matcher{screenshot};
        position = matcher.search(target);
        if (position) {
            break;
        } else {
            std::printf("游戏正在更新\n");
            std::this_thread::sleep_for(1'000ms);
        }
    }
    std::printf("游戏已更新\n");
}