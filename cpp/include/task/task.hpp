#ifndef KOALAYT_TASK_HPP_20210409
#define KOALAYT_TASK_HPP_20210409

// std
#include <vector>
// project
#include <window/window.hpp>
#include <imagesearch/match.hpp>

namespace autozhuxian {

///
/// 某种操作
/// ---------------------------------------------------------
/// 作为所有的操作的base class，里面定义了一些公共的方法
///
class Command {
public:
    // ---------------------------------------------------------
    // Constructor
    //
    Command(const char* name) : m_name{name} {}

    // ---------------------------------------------------------
    // Destructor
    //
    virtual ~Command() {}

    // ---------------------------------------------------------
    // 在某个窗口中执行这一操作
    //
    virtual bool execute(Window& win) = 0;

protected:
    const char* m_name;  // 操作名称

    // ---------------------------------------------------------
    // 等待一段时间
    //
    void wait(int ms);
};

///
/// 根据图片，找到其在窗口中的位置，并点击
/// ---------------------------------------------------------
/// 比如右下角的系统、副本等按钮，点击报名各种任务的icon
///
class ClickByImageCmd : public Command {
public:
    using ImageSearchTargets = std::vector<ImageSearchTarget>;

    // ---------------------------------------------------------
    // Constructor1
    //
    ClickByImageCmd(const char*          name,
                    RegionOfInterest     roi,
                    ImageSearchTargets&& targets,
                    int                  wait)
        : Command{name},
          m_roi{roi},
          m_targets{std::move(targets)},
          m_wait{wait}
    {}

    // ---------------------------------------------------------
    // Constructor2: 大部分情况下，就只要搜索一张图片
    //
    ClickByImageCmd(const char*      name,
                    RegionOfInterest roi,
                    const char*      target,
                    int              wait)
        : Command{name},
          m_roi{roi},
          m_wait{wait}
    {
        m_targets.emplace_back(target);
    }

    // Destructor
    // ---------------------------------------------------------
    ~ClickByImageCmd() override = default;

    // Dispatcher
    // ---------------------------------------------------------
    bool execute(Window& win) override;

private:
    RegionOfInterest   m_roi;      // 在窗口的这个区域内搜索
    ImageSearchTargets m_targets;  // 要搜索的图片，只要有一个target符合，就算搜索成功
    int                m_wait;     // 点击完成后，由于场景切换等原因，需要等待UI变化的时间（毫秒）
};

///
/// 点击窗口中固定的某个位置
/// ---------------------------------------------------------
/// 有些场景的UI有特效，或者是半透明的UI，且背景实时变化，
/// 用图像匹配很难保证100%正确，好在位置是绝对固定的，就直接硬编码位置
///
class ClickByPositionCmd : public Command {
public:
    ClickByPositionCmd(const char* name,
                       cv::Point   position,
                       int         wait)
        : Command{name},
          m_position{position},
          m_wait{wait}
    {}

    ~ClickByPositionCmd() override = default;

    bool execute(Window& win) override;

private:
    cv::Point m_position;  // 相对窗口的坐标
    int       m_wait;      // 点击完成后，等待的时间
};

};  // namespace autozhuxian

#endif