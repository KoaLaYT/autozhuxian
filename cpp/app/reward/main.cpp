// std
#include <iostream>
// project
#include <task/common.hpp>
#include <task/command.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\reward\\"
#define PATH(p) BASE p

using namespace autozhuxian;

class RewardTask : public Logger<RewardTask> {
public:
    void run(Window& win)
    {
        log_block(win.role_name(), "领取奖励");

        common_task::activate(win);
        common_task::close_all_ui(win);
        common_task::open_ui(win, common_task::UIType::Reward);

        std::vector<std::unique_ptr<Command>> cmds;
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("点击签到奖励",
                                                            ImageSearchTargets{{PATH("signup.png")},
                                                                               {PATH("signup_selected.png")}},
                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("领取签到奖励",
                                                            PATH("signup_confirm.png"),
                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("点击升级奖励",
                                                            ImageSearchTargets{{PATH("levelup.png")},
                                                                               {PATH("levelup_selected.png")}},
                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("领取升级奖励",
                                                            PATH("levelup_confirm.png"),
                                                            200));
        for (auto& cmd : cmds) cmd->execute(win);

        close_ui(win, common_task::UIType::Reward);
    }
};

int main()
{
    auto       wins = common_task::find_all_zx_wins();
    RewardTask task;

    for (auto& win : wins) {
        task.run(win);
    }

    return 0;
}