#include <control/control.hpp>
#include <array>

namespace autozhuxian {

void press(DWORD keycode)
{
    std::array<INPUT, 2> inputs;

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = keycode;
    inputs[0].ki.dwFlags = 0;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keycode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

};  // namespace autozhuxian