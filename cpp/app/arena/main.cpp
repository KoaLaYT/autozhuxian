// std
#include <vector>
// project
#include <task/stateful.hpp>
#include <task/common.hpp>
#include <task/command.hpp>
#include <control/control.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\arena\\"
#define PATH(p) BASE p

using namespace autozhuxian;

enum class State {
    Signup,     // 报名
    Entering,   // 等待进入
    Finishing,  // 等待结束（失败、成功暂时无法判断）
    TOTAL
};

// 报名时执行的命令流
class SignupStrategy : public Strategy {
public:
    int run(Window& win) override
    {
        std::vector<std::unique_ptr<Command>> cmds;
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("点击莲台问道",
                                                            PATH("step_1_1.png"),
                                                            1000));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("同门挑战",
                                                            PATH("step_1_2.png"),
                                                            1000));
        bool success = false;
        common_task::press_esc();
        common_task::open_ui(win, common_task::UIType::PVP);
        for (auto& cmd : cmds) {
            success = cmd->execute(win);
            if (!success) break;
        }
        // 失败后把执行权让给其他窗口
        // 避免这个窗口一直失败，导致别的窗口无法正常运行
        return success ? 0 : 1000;
    };
};

// 等待进入时执行的命令流
class EnteringStrategy : public Strategy {
public:
    int run(Window& win) override
    {
        ConfirmImageCmd cmd{"是否进入战场", PATH("step_2_1.png")};

        common_task::press_esc();
        press(0x4D);  // M
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto found = cmd.execute(win);
        common_task::press_esc();

        return found ? 0 : 10'000;
    }
};

// 结束后执行的命令流
// TODO 直接退出？
class FinishingStrategy : public Strategy {
public:
    int run(Window& win) override
    {
        ConfirmImageCmd cmd{"是否回到河阳城", PATH("step_3_1.png")};

        common_task::press_esc();
        press(0x4D);  // M
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto found = cmd.execute(win);
        common_task::press_esc();

        return found ? 0 : 15'000;
    }
};

class Task : public StatefulTask<State> {
public:
    explicit Task(Window& win) : StatefulTask<State>{win, State::Signup}
    {
        m_strategies[State::Signup] = std::make_unique<SignupStrategy>();
        m_strategies[State::Entering] = std::make_unique<EnteringStrategy>();
        m_strategies[State::Finishing] = std::make_unique<FinishingStrategy>();
    }

private:
    void next_state() override
    {
        int total = static_cast<int>(State::TOTAL);
        int i = static_cast<int>(m_state);
        i += 1;
        m_state = static_cast<State>(i % total);
    }

    const char* state_name() override
    {
        switch (m_state) {
            case State::Signup:
                return "报名";
            case State::Entering:
                return "等待进入";
            default:
                return "等待完成";
        }
    }
};

int main()
{
    auto wins = common_task::find_all_zx_wins();

    Scheduler<Task> scheduler{"莲台问道", 88, wins};
    scheduler.run();

    return 0;
}