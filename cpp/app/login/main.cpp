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

// TODO remove duplicate logic
static void open_platform();
// static void start_game(HWND hwnd);

struct WindowManager {
    using ZXWindow = autozhuxian::Window;

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

int main()
{
    std::vector<autozhuxian::Process> task{
        {
            "寻找服务器",
            {400, 0, 800, 1024},
            PATH("server.png"),
            NULL,
            0,
            nullptr,
        },
        {
            "选择服务器",
            {400, 0, 800, 1024},
            PATH("confirm.png"),
            NULL,
            10'000,  // 人物有个动画效果
            nullptr,
        },
        {
            "进入游戏",
            {400, 512, 800, 512},
            PATH("enter.png"),
            PATH("enter_mask.png"),
            10'000,
            nullptr,
        },
    };

    char* platform = "完美游戏平台";
    char* platform_subwin = "游戏多开";
    auto platform_hwnd = autozhuxian::find_window(platform);
    if (!platform_hwnd) {
        std::printf("未找到窗口，尝试打开窗口\n");
        open_platform();
        return main();
    }
    std::printf("完美游戏平台已打开\n");
    // start_game(platform_hwnd);
    // std::printf("诛仙已开始\n");
    // WindowManager wm;

    // for (auto& win : wm.windows) {
    //     // 切换窗口，等待界面刷新
    //     SetForegroundWindow(win.handle());
    //     std::this_thread::sleep_for(1000ms);

    //     for (auto& proc : task) {
    //         if (!win.run(proc)) {
    //             std::printf("子流程%s失败\n", proc.name);
    //             //TODO back to last process
    //         }
    //         // TODO proc.confirm
    //     }
    // }

    return 0;
}

static void open_platform()
{
    autozhuxian::press(VK_LWIN);
    // waiting for the window shows
    std::this_thread::sleep_for(2000ms);
    autozhuxian::move(0, 0);

    HWND desktop_hwnd = GetDesktopWindow();
    cv::Mat source = autozhuxian::screenshot_region(desktop_hwnd,
                                                    autozhuxian::RegionOfInterest::from_hwnd(desktop_hwnd));
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

/*
static void start_game(HWND hwnd)
{
    SetForegroundWindow(hwnd);
    auto region = get_window_region(hwnd);
    auto position = get_window_position(hwnd);
    cv::Mat platform_screenshot = autozhuxian::screenshot_region(hwnd, region);
    cv::Mat start_game_icon = cv::imread(PATH("login\\start_game.png"), cv::IMREAD_UNCHANGED);
    cv::Point location = autozhuxian::match_template(platform_screenshot, start_game_icon);

    autozhuxian::click(location.x + position.left + start_game_icon.cols / 2,
                       location.y + position.top + start_game_icon.rows / 2);
    // waiting for the window shows
    std::this_thread::sleep_for(30'000ms);
}

static void select_server(HWND hwnd)
{
    SetForegroundWindow(hwnd);
    autozhuxian::move(0, 0);

    auto region = get_window_region(hwnd);
    auto position = get_window_position(hwnd);
    cv::Mat platform_screenshot = autozhuxian::screenshot_region(hwnd, region);
    {
        cv::Mat start_game_icon = cv::imread(PATH("login\\server.png"), cv::IMREAD_UNCHANGED);
        cv::Point location = autozhuxian::match_template(platform_screenshot, start_game_icon);

        // TODO find more accurate calculation of window caption bar size
        autozhuxian::click(location.x + position.left + start_game_icon.cols / 2,
                           location.y + position.top + start_game_icon.rows / 2 + position.height - region.height);
    }
    {
        cv::Mat start_game_icon = cv::imread(PATH("login\\confirm.png"), cv::IMREAD_UNCHANGED);
        cv::Point location = autozhuxian::match_template(platform_screenshot, start_game_icon);

        // TODO find more accurate calculation of window caption bar size
        autozhuxian::click(location.x + position.left + start_game_icon.cols / 2,
                           location.y + position.top + start_game_icon.rows / 2 + position.height - region.height);
    }
    // waiting for the window shows
    std::this_thread::sleep_for(2'000ms);
}

static void enter_game(HWND hwnd)
{
    autozhuxian::move(0, 0);
    auto region = get_window_region(hwnd);
    auto position = get_window_position(hwnd);
    cv::Mat platform_screenshot = autozhuxian::screenshot_region(hwnd, region);

    cv::imwrite(PATH("tmp\\enter.png"), platform_screenshot);

    cv::Mat start_game_icon = cv::imread(PATH("login\\enter.png"), cv::IMREAD_UNCHANGED);
    cv::Point location = autozhuxian::match_template(platform_screenshot, start_game_icon);

    // TODO find more accurate calculation of window caption bar size
    autozhuxian::click(location.x + position.left + start_game_icon.cols / 2,
                       location.y + position.top + start_game_icon.rows / 2 + position.height - region.height);
}

static autozhuxian::ScreenshotRegion get_window_region(HWND hwnd)
{
    RECT window_rect;
    GetClientRect(hwnd, &window_rect);
    return autozhuxian::ScreenshotRegion{window_rect};
}

static autozhuxian::ScreenshotRegion get_window_position(HWND hwnd)
{
    RECT window_rect;
    GetWindowRect(hwnd, &window_rect);
    return autozhuxian::ScreenshotRegion{window_rect};
}
*/