// std
#include <thread>
// project
#include <task/task.hpp>
#include <control/control.hpp>
#include <imagesearch/screenshot.hpp>
#include <imagesearch/match.hpp>

namespace autozhuxian {

///
/// 所有操作的公共方法
/// ---------------------------------------------------------
/// 等待一段时间
///
void Command::wait(int ms)
{
    if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

///
/// 操作类型 1
/// ---------------------------------------------------------
/// 根据图片，找到其在窗口中的位置，并点击
///
bool ClickByImageCmd::execute(Window& win)
{
    // TODO logger, move to Command
    std::printf("\t执行操作：%s\n", m_name);
    // 鼠标移到左上角，防止影响图像匹配
    // ---------------------------------------------------------
    move(0, 0);

    // 对搜索区域截图
    // ---------------------------------------------------------
    cv::Mat roi_screenshot = screenshot_region(win.handle(),
                                               m_roi.is_whole() ? win.roi() : m_roi);
    Matcher matcher{roi_screenshot};

    // 搜索目标图片的坐标
    // ---------------------------------------------------------
    std::optional<cv::Point> location = std::nullopt;
    auto ist = m_targets.begin();
    while (ist != m_targets.end()) {
        location = ist->has_mask() ? matcher.search_with_mask(ist->target(), ist->mask())
                                   : matcher.search(ist->target());
        if (location) break;
        ist++;
    }

    // 搜索结束
    // ---------------------------------------------------------
    if (!location) {
        std::printf("\t\t失败，未找到目标图片\n");
        return false;
    }
    // TODO logger
    std::printf("\t\t找到目标图片，位置(%d, %d)\n", location->x, location->y);

    // 点击目标图片的中心
    // = 窗口偏移 + roi + location + 目标图片尺寸 / 2
    // ---------------------------------------------------------
    int x = win.x() + m_roi.x + location->x + ist->width() / 2;
    int y = win.y() + m_roi.y + location->y + ist->height() / 2;
    // TODO logger
    std::printf("\t\t鼠标点击(%d, %d)\n", x, y);
    click(x, y);

    // 等待UI变化
    // ---------------------------------------------------------
    Command::wait(m_wait);

    return true;
}

///
/// 操作类型 2
/// ---------------------------------------------------------
/// 点击窗口中固定的某个位置
///
bool ClickByPositionCmd::execute(Window& win)
{
    // TODO logger, move to Command
    std::printf("\t执行操作：%s\n", m_name);

    // 点击目标位置
    // ---------------------------------------------------------
    int x = win.x() + m_position.x;
    int y = win.y() + m_position.y;
    // TODO logger
    std::printf("\t\t鼠标点击(%d, %d)\n", x, y);
    click(x, y);

    // 等待UI变化
    // ---------------------------------------------------------
    Command::wait(m_wait);

    return true;
}

};  // namespace autozhuxian