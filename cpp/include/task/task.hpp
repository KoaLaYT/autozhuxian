#ifndef KOALAYT_TASK_HPP_20210409
#define KOALAYT_TASK_HPP_20210409

// std
#include <vector>
// project
#include <window/window.hpp>
#include <imagesearch/match.hpp>

namespace autozhuxian {

/**
 * @brief 某种操作
 * 
 */
class Command {
public:
    Command(const char* name) : m_name{name} {}
    virtual ~Command() {}

    // 在某个窗口中执行这一操作
    virtual bool execute(Window& win) = 0;

protected:
    const char* m_name;  // 操作名称
};

/**
 * @brief 根据图片，找到其在窗口中的位置，并点击
 * 
 * 比如右下角的系统、副本等按钮，点击报名各种任务的icon
 * 
 */
class ClickByImageCmd : public Command {
public:
    using ImageSearchTargets = std::vector<ImageSearchTarget>;

    ClickByImageCmd(const char*          name,
                    RegionOfInterest     roi,
                    ImageSearchTargets&& targets,
                    int                  wait)
        : Command{name},
          m_roi{roi},
          m_targets{std::move(targets)},
          m_wait{wait}
    {}

    // 大部分情况下，就只要搜索一张图片
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

    ~ClickByImageCmd() override = default;

    bool execute(Window& win) override;

private:
    RegionOfInterest   m_roi;      // 在窗口的这个区域内搜索
    ImageSearchTargets m_targets;  // 要搜索的图片，只要有一个target符合，就算搜索成功
    int                m_wait;     // 点击完成后，由于场景切换等原因，需要等待UI变化的时间（毫秒）
};

};  // namespace autozhuxian

#endif