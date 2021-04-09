#ifndef KOALAYT_WINDOW_HPP_20210407
#define KOALAYT_WINDOW_HPP_20210407

#include <windows.h>
#include <winuser.h>

#include <optional>
#include <string>

namespace autozhuxian {

/**
 * @brief 对win32窗口信息的封装
 * 
 */
class Window {
public:
    Window(char* title, HWND handle) : m_title{title},
                                       m_handle{handle}
    {
        m_window_info.cbSize = sizeof(WINDOWINFO);
        if (!GetWindowInfo(m_handle, &m_window_info)) {
            throw std::runtime_error{"GetWindowInfo Failed\n"};
        }
    }

    // TODO add more useful methods

private:
    std::string m_title;       // 窗口标题
    HWND m_handle;             // 窗口handle
    WINDOWINFO m_window_info;  // 窗口信息
};

/**
 * @brief 根据标题找到对应的窗口信息
 * 
 * @param title 窗口标题
 * @return std::optional<Window> 
 */
std::optional<Window> find_window(PCHAR title);

};  // namespace autozhuxian

#endif