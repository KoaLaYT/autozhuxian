#ifndef KOALAYT_CONTROL_HPP_20210407
#define KOALAYT_CONTROL_HPP_20210407

#include <windows.h>
#include <winuser.h>

namespace autozhuxian {

/**
 * @brief 模拟按键
 * 
 * @param keycode 
 */
void press(DWORD keycode);

};  // namespace autozhuxian

#endif