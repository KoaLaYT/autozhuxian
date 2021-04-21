// std
#include <thread>
// project
#include <command/command.hpp>
#include <control/control.hpp>
#include <imagesearch/screenshot.hpp>
#include <imagesearch/match.hpp>

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

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
/// 所有操作的公共方法
/// ---------------------------------------------------------
/// 在窗口的ROI中搜索目标图片的位置
///
Command::FindLocRes Command::find_location(Window&             win,
                                           RegionOfInterest&   roi,
                                           ImageSearchTargets& targets)
{
    // 鼠标移到左上角，防止影响图像匹配
    // ---------------------------------------------------------
    move(0, 0);

    // 对搜索区域截图
    // ---------------------------------------------------------
    cv::Mat roi_screenshot = screenshot_region(win.handle(),
                                               roi.is_whole() ? win.roi() : roi);
    Matcher matcher{roi_screenshot};

    // 搜索目标图片的坐标
    // ---------------------------------------------------------
    std::optional<cv::Point> location = std::nullopt;
    auto                     ist = targets.begin();
    while (ist != targets.end()) {
        location = ist->has_mask() ? matcher.search_with_mask(ist->target(), ist->mask())
                                   : matcher.search(ist->target());
        if (location) break;
        ist++;
    }

    // 搜索结束
    // ---------------------------------------------------------
    if (!location) {
        info("<{}> 失败，未找到目标图片", win.role_name());
        return FindLocRes{};
    } else {
        info("<{}> 找到目标图片，位置({}, {})", win.role_name(), location->x, location->y);
        return FindLocRes{location, ist->width(), ist->height()};
    }
}

///
/// 操作类型 1
/// ---------------------------------------------------------
/// 根据图片，找到其在窗口中的位置，并点击
///
bool ClickByImageCmd::execute(Window& win)
{
    // 寻找目标坐标
    // ---------------------------------------------------------
    auto result = find_location(win, m_roi, m_targets);
    if (!result.loc) {
        info("<{}> 执行操作 [{}] 失败", win.role_name(), m_name);
        return false;
    }

    // 点击目标图片的中心
    // = 窗口偏移 + roi + location + 目标图片尺寸 / 2
    // ---------------------------------------------------------
    int x = win.x() + m_roi.x + result.loc->x + result.width / 2;
    int y = win.y() + m_roi.y + result.loc->y + result.height / 2;
    click(x, y);

    // 等待UI变化
    // ---------------------------------------------------------
    Command::wait(m_wait);

    info("<{}> 执行操作 [{}] 成功（图片坐标 {}，{}）", win.role_name(), m_name, x, y);
    return true;
}

///
/// 操作类型 2
/// ---------------------------------------------------------
/// 点击窗口中固定的某个位置
///
bool ClickByPositionCmd::execute(Window& win)
{
    // 点击目标位置
    // ---------------------------------------------------------
    int x = win.x() + m_position.x;
    int y = win.y() + m_position.y;
    click(x, y);

    // 等待UI变化
    // ---------------------------------------------------------
    Command::wait(m_wait);

    info("<{}> 执行操作 [{}] 成功（点击坐标 {}，{}）", win.role_name(), m_name, x, y);
    return true;
}

///
/// 操作类型 3
/// ---------------------------------------------------------
/// 查看窗口中某个图片是否出现
///
bool ConfirmImageCmd::execute(Window& win)
{
    // TODO logger, move to Command
    std::printf("\t执行操作：%s\n", m_name);

    auto result = find_location(win, m_roi, m_targets);
    bool is_success = result.loc.has_value();

    info("<{}> 执行操作 [{}] {}", win.role_name(), m_name, is_success ? "成功" : "失败");
    return is_success;
}

};  // namespace autozhuxian