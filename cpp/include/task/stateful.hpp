#ifndef KOALAYT_STATEFUL_HPP_20210422
#define KOALAYT_STATEFUL_HPP_20210422

// std
#include <thread>
#include <chrono>
#include <map>
// #include <iomanip>
#include <ctime>
// project
#include <logger/logger.hpp>
#include <window/window.hpp>
#include <task/common.hpp>

using namespace std::chrono_literals;

namespace impl {
///
/// 打印时间
/// ---------------------------------------------------------
/// 用于日志输出
///
template <typename T>
std::string time_to_str(const std::chrono::time_point<T>& time)
{
    auto     c_t = std::chrono::system_clock::to_time_t(time);
    std::tm* tm = std::localtime(&c_t);

    std::ostringstream oss;
    oss << std::put_time(tm, "%F %T");
    return oss.str();
}

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

namespace autozhuxian {
///
/// 一个任务，当前状态下，需要执行的命令流
/// ---------------------------------------------------------
///
///
class Strategy : public Logger<Strategy> {
public:
    // ---------------------------------------------------------
    // Destructor
    //
    virtual ~Strategy() = default;

    // ---------------------------------------------------------
    // 返回要等待的毫秒
    // 0 表示成功
    // > 0 表示等待多少毫秒后重试
    //
    virtual int run(Window& win) = 0;
};

///
/// 带状态的任务
/// ---------------------------------------------------------
/// 每个状态对应一种Strategy，即执行对应的命令流
/// 成功后切换到下一状态，否则在等待一段时间后重试
///
template <typename State>
class StatefulTask : public Logger<StatefulTask<State>> {
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    // ---------------------------------------------------------
    // Constructor
    //
    StatefulTask(Window& win, State init)
        : m_win{win},
          m_state{init}
    {}

    // ---------------------------------------------------------
    // Destructor
    //
    virtual ~StatefulTask() = default;

    // ---------------------------------------------------------
    // 执行该命令流，返回值是下次运行的时间
    //
    TimePoint run()
    {
        Logger<StatefulTask<State>>::info("<{}> 尝试 [{}]", m_win.role_name(), state_name());

        // 尝试执行当前的命令流
        // ---------------------------------------------------------
        common_task::activate(m_win);
        int  wait = m_strategies[m_state]->run(m_win);
        auto t = std::chrono::system_clock::now() + std::chrono::milliseconds(wait);

        std::string status = wait == 0 ? "成功" : "失败";
        Logger<StatefulTask<State>>::info("<{}> [{}] {}，下次运行时间 {}",
                                          m_win.role_name(), state_name(), status, impl::time_to_str(t));
        // 进入下一阶段
        // ---------------------------------------------------------
        if (wait == 0) next_state();

        return t;
    }

protected:
    Window&                                    m_win;         // 每个窗口有自己的任务状态
    State                                      m_state;       // 该任务的当前状态
    std::map<State, std::unique_ptr<Strategy>> m_strategies;  // 不同状态对应的命令流

    // ---------------------------------------------------------
    // 进入下一个状态
    //
    virtual void next_state() = 0;

    // ---------------------------------------------------------
    // 状态的名字，日志输出需要
    //
    virtual const char* state_name() = 0;
};

///
/// 各个窗口任务的调度
/// ---------------------------------------------------------
/// 每个窗口的任务跑过一遍后，需要等待一段时间
/// 因为大部分时候在等待进入66后者在66里挂机等结束，所以没必要马上进入下一个循环
/// 现在想到下面几种方法：
/// 1. 简单：直接等1-2s （可能不够普适，66/莲花适合）
/// 2. 精细点：在每个strategy里设定好下次要跑的时间，在运行前检查下（busy waiting，好像很浪费）
/// 3. 多线程：不确定界面在线程切换的时候，被调到后台后，截图是否还会更新，还有模拟按键能不能收到
///
/// 4. 现在使用时的方法如下：是一种2的改良版
///    每个task都记录下一次运行的时间，如果小于当前时间，在这个循环可以运行
///    task.run返回下一次运行的时间，更新map里的记录
///    获取所有下一次运行时间的最小值next_round，如果大于当前时间，那么就休眠当前线程至next_round
template <typename StatefulTask>
class Scheduler : public Logger<Scheduler<StatefulTask>> {
public:
    // ---------------------------------------------------------
    // Constructor: 每个窗口绑定一个StatefulTask
    //
    Scheduler(const char*          name,
              int                  duration,  // 任务持续的时间（单位：分）
              std::vector<Window>& wins)
        : m_name{name}
    {
        auto now = std::chrono::system_clock::now();
        m_endtime = now + std::chrono::minutes(duration);
        for (auto& win : wins) {
            auto task = new StatefulTask{win};
            m_tasks[task] = now;
        }
    }

    // ---------------------------------------------------------
    // Destructor: 清理所有的StatefulTask
    //
    ~Scheduler()
    {
        for (auto& pair : m_tasks) {
            delete pair.first;
        }
    }

    // ---------------------------------------------------------
    // 开始在所有窗口间循环运行任务
    //
    void run()
    {
        Logger<Scheduler<StatefulTask>>::log_block(m_name);
        std::this_thread::sleep_for(1000ms);  // 防止输出日志乱序

        while (std::chrono::system_clock::now() < m_endtime) {
            // 遍历每个任务，记录最新的等待时间
            // 并且找到下次运行时间的最近值
            // ---------------------------------------------------------
            auto next_round = StatefulTask::TimePoint::max();
            for (auto& pair : m_tasks) {
                if (pair.second <= std::chrono::system_clock::now()) {
                    m_tasks[pair.first] = pair.first->run();
                }
                next_round = m_tasks[pair.first] < next_round ? m_tasks[pair.first] : next_round;
            }
            // 根据最近值，停止当前线程
            // ---------------------------------------------------------
            if (next_round > std::chrono::system_clock::now()) {
                Logger<Scheduler<StatefulTask>>::info("线程休眠至 {}", impl::time_to_str(next_round));
                std::this_thread::sleep_until(next_round);
            }
        }
    }

private:
    const char*                                               m_name;     // 任务名
    typename StatefulTask::TimePoint                          m_endtime;  // 任务结束的时间
    std::map<StatefulTask*, typename StatefulTask::TimePoint> m_tasks;    // 任务 -> 下次运行的时间
};

};  // namespace autozhuxian

#endif