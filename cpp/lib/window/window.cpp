#include <cstring>
#include <cstdio>

#include <window/window.hpp>
#include <imagesearch/screenshot.hpp>
#include <imagesearch/match.hpp>
#include <control/control.hpp>
#include <thread>

namespace impl {

struct FindWindowResult {
    PCHAR title;
    HWND handle;
};

/**
 * @brief find_window的回调函数，在找到第一个匹配title的窗口后就停止
 * 
 * @param hwnd 当前遍历到的窗口
 * @param lParam EnumWindows传入的参数
 * @return TRUE 继续下一个窗口
 *         FALSE 停止
 */
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

    auto result = reinterpret_cast<FindWindowResult*>(lParam);
    if (std::strcmp(title, result->title) == 0) {
        result->handle = hwnd;
        return FALSE;
    }

    return TRUE;
}

};  // namespace impl

namespace autozhuxian {

std::optional<Window> find_window(PCHAR title)
{
    impl::FindWindowResult result{title, nullptr};

    EnumWindows(impl::EnumWindowCb, reinterpret_cast<LPARAM>(&result));

    if (result.handle) {
        return std::optional{Window{title, result.handle}};
    } else {
        return std::nullopt;
    }
}

bool Window::run(Process& process)
{
    std::printf("开始子流程：%s\n", process.name);
    // 鼠标移到左上角，防止影响图像匹配
    move(0, 0);

    cv::Mat roi_screenshot = screenshot_region(m_handle, process.roi);
    cv::Mat target = cv::imread(process.target, cv::IMREAD_UNCHANGED);

    Matcher matcher{roi_screenshot};
    std::optional<cv::Point> location = std::nullopt;
    if (process.mask) {
        cv::Mat mask = cv::imread(process.mask, cv::IMREAD_GRAYSCALE);
        location = matcher.search_with_mask(target, mask);
    } else {
        location = matcher.search(target);
    }
    if (!location) {
        std::printf("子流程%s 失败，未找到目标图片\n", process.name);
        return false;
    }
    std::printf("找到目标图片，位置(%d, %d)\n", location->x, location->y);
    // TODO 实现command，先简单做
    // 点击目标图片的中心 = m_handle + roi + location + 目标图片尺寸 / 2
    // 注意要减去window的caption 和 border
    int x = m_window_info.rcWindow.left + process.roi.x + location->x + target.cols / 2;
    int y = m_window_info.rcWindow.top + process.roi.y + location->y + target.rows / 2 + window_cap_border_height();
    std::printf("鼠标点击(%d,%d)\n", x, y);
    click(x, y);
    // 等待UI变化
    if (process.wait) {
        std::this_thread::sleep_for(std::chrono::milliseconds(process.wait));
    }

    return true;
}

};  // namespace autozhuxian