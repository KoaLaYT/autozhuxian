#ifndef KOALAYT_CONTROL_HPP_20210407
#define KOALAYT_CONTROL_HPP_20210407

// win32
#include <windows.h>
#include <winuser.h>

namespace autozhuxian {

///
/// 模拟按键
/// ---------------------------------------------------------
/// keycode对应表：https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
///
void press(DWORD keycode);

///
/// 鼠标移动（相对屏幕的绝对位置）
/// ---------------------------------------------------------
///
///
void move(int x, int y);

///
/// 鼠标点击当前位置
/// ---------------------------------------------------------
///
///
void click();

///
/// 鼠标点击相对屏幕的绝对位置
/// ---------------------------------------------------------
///
///
void click(int x, int y);

};  // namespace autozhuxian

#endif