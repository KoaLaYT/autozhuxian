#include <window/window.hpp>
#include <control/control.hpp>
#include <imagesearch/screenshot.hpp>
#include <imagesearch/match.hpp>
#include <cstdio>
#include <thread>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\"
#define PATH(p) BASE p

using namespace std::chrono_literals;

// TODO remove duplicate logic
static void open_platform();
static void start_game(HWND hwnd);
static void select_server(HWND hwnd);
static void enter_game(HWND hwnd);
static autozhuxian::ScreenshotRegion get_window_region(HWND hwnd);
static autozhuxian::ScreenshotRegion get_window_position(HWND hwnd);

int main()
{
    char* platform = "完美游戏平台";
    HWND platform_hwnd = autozhuxian::find_window(platform);
    if (!platform_hwnd) {
        std::printf("未找到窗口，尝试打开窗口\n");
        open_platform();
        return main();
    }
    std::printf("完美游戏平台已打开\n");
    start_game(platform_hwnd);
    std::printf("诛仙已开始\n");

    HWND zhuxian_hwnd = autozhuxian::find_window("诛仙3");
    if (!zhuxian_hwnd) {
        // ???
        return 1;
    }
    select_server(zhuxian_hwnd);
    enter_game(zhuxian_hwnd);

    return 0;
}

static void open_platform()
{
    autozhuxian::press(VK_LWIN);
    // waiting for the window shows
    std::this_thread::sleep_for(2000ms);

    HWND desktop_hwnd = GetDesktopWindow();
    auto desktop_region = get_window_region(desktop_hwnd);
    cv::Mat source = autozhuxian::screenshot_region(desktop_hwnd, desktop_region.left_bottom());
    cv::Mat platform_icon = cv::imread(PATH("login\\wanmei_platform.png"), cv::IMREAD_UNCHANGED);
    cv::Point location = autozhuxian::match_template(source, platform_icon);

    autozhuxian::click(location.x + platform_icon.rows / 2,
                       location.y + desktop_region.height / 2 + platform_icon.cols / 2);

    // waiting for the window shows
    std::this_thread::sleep_for(5000ms);
}

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