// std
#include <array>
// project
#include <control/control.hpp>

namespace impl {

///
/// 获取屏幕的尺寸
/// ---------------------------------------------------------
///
///
static std::pair<int, int> get_desktop_size()
{
    HWND desktop = GetDesktopWindow();
    RECT rect;
    GetClientRect(desktop, &rect);
    return std::pair<int, int>{rect.right - rect.left,
                               rect.bottom - rect.top};
}

};  // namespace impl

namespace autozhuxian {

///
/// 控制
/// ---------------------------------------------------------
/// 模拟按键
///
void press(DWORD keycode)
{
    std::array<INPUT, 2> inputs;
    inputs.fill({});  //! IMPORTANT，详见docs/zero_initialize.md

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = keycode;
    inputs[0].ki.dwFlags = 0;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keycode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

///
/// 控制
/// ---------------------------------------------------------
/// 鼠标移动（相对屏幕的绝对位置）
///
void move(int x, int y)
{
    auto size = impl::get_desktop_size();

    INPUT input{};  //! IMPORTANT，详见docs/zero_initialize.md
    input.type = INPUT_MOUSE;
    // 根据msdn文档，需要作这样的转换
    input.mi.dx = (double)x / size.first * 65536;
    input.mi.dy = (double)y / size.second * 65536;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

///
/// 控制
/// ---------------------------------------------------------
/// 鼠标点击当前位置
///
void click()
{
    std::array<INPUT, 2> inputs;
    inputs.fill({});  //! IMPORTANT，详见docs/zero_initialize.md

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = 0;
    inputs[0].mi.dy = 0;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dx = 0;
    inputs[1].mi.dy = 0;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

///
/// 控制
/// ---------------------------------------------------------
/// 鼠标点击相对屏幕的绝对位置
///
void click(int x, int y)
{
    move(x, y);
    click();
}

};  // namespace autozhuxian