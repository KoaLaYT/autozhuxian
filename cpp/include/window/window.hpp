#ifndef KOALAYT_WINDOW_HPP_20210407
#define KOALAYT_WINDOW_HPP_20210407

#include <windows.h>
#include <winuser.h>

namespace autozhuxian {

/**
 * @brief 根据窗口标题找到对应的windows handle
 * 
 * @param title 窗口标题
 * @return HWND windows handle OR nullptr
 */
HWND find_window(PCHAR title);

};  // namespace autozhuxian

#endif