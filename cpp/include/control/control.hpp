#ifndef KOALAYT_CONTROL_HPP_20210407
#define KOALAYT_CONTROL_HPP_20210407

// win32
#include <windows.h>
#include <winuser.h>

namespace autozhuxian {

/**
 * @brief 模拟按键
 * 
 * @param keycode 
 */
void press(DWORD keycode);

/**
 * @brief 鼠标移动（相对屏幕的绝对位置）
 * 
 * @param x 
 * @param y 
 */
void move(int x, int y);

/**
 * @brief 鼠标点击当前位置
 * 
 */
void click();

/**
 * @brief 鼠标点击相对屏幕的绝对位置
 * 
 * @param x 
 * @param y 
 */
void click(int x, int y);

};  // namespace autozhuxian

#endif