// std
#include <iostream>
// project
#include <util/common_task.hpp>
#include <command/command.hpp>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\assets\\reward\\"
#define PATH(p) BASE p

using namespace autozhuxian;

class RewardTask {
public:
    void run(Window& win)
    {
        SetForegroundWindow(win.handle());
        util::close_all_ui(win);
        util::open_ui(win, util::UIType::Reward);

        std::vector<std::unique_ptr<Command>> cmds;
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("点击签到奖励",
                                                            RegionOfInterest::whole,
                                                            ImageSearchTargets{{PATH("signup.png")},
                                                                               {PATH("signup_selected.png")}},

                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("领取签到奖励",
                                                            RegionOfInterest::whole,
                                                            PATH("signup_confirm.png"),
                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("点击升级奖励",
                                                            RegionOfInterest::whole,
                                                            ImageSearchTargets{{PATH("levelup.png")},
                                                                               {PATH("levelup_selected.png")}},
                                                            200));
        cmds.emplace_back(std::make_unique<ClickByImageCmd>("领取升级奖励",
                                                            RegionOfInterest::whole,
                                                            PATH("levelup_confirm.png"),
                                                            200));
        for (auto& cmd : cmds) cmd->execute(win);
        util::close_ui(win, util::UIType::Reward);
    }
};

int main()
{
    auto       wins = util::find_all_zx_wins();
    RewardTask task;

    for (auto& win : wins) {
        task.run(win);
    }

    return 0;
}