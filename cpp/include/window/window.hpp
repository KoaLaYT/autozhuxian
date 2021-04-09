#ifndef KOALAYT_WINDOW_HPP_20210407
#define KOALAYT_WINDOW_HPP_20210407

#include <windows.h>
#include <winuser.h>

#include <optional>
#include <string>
#include <stdexcept>

#include <task/task.hpp>

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

    HWND handle() { return m_handle; }

    /**
     * // TODO
     * @brief 
     * 
     * @param process 
     * @return true 
     * @return false 
     */
    bool run(Process& process);

    // TODO add more useful methods

private:
    std::string m_title;       // 窗口标题
    HWND m_handle;             // 窗口handle
    WINDOWINFO m_window_info;  // 窗口信息

    int rect_height(RECT& rect) { return rect.bottom - rect.top; }

    int window_cap_border_height()
    {
        int win_height = rect_height(m_window_info.rcWindow);
        int cli_height = rect_height(m_window_info.rcClient);

        return win_height - cli_height - m_window_info.cyWindowBorders;
    }
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