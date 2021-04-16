#ifndef KOALAYT_WINDOW_HPP_20210407
#define KOALAYT_WINDOW_HPP_20210407

// std
#include <optional>
#include <stdexcept>
#include <string>
// win32
#include <windows.h>
#include <winuser.h>
// project
#include <imagesearch/screenshot.hpp>

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

    Window() : m_title{""}, m_handle{nullptr} {}

    // getter
    HWND       handle() { return m_handle; }
    WINDOWINFO window_info() { return m_window_info; }

    // 窗口本身的位置，在相对坐标转绝对坐标时，需要加上
    int x() { return m_window_info.rcWindow.left + m_window_info.cxWindowBorders; }
    int y() { return m_window_info.rcWindow.top + title_and_border_height(); }

    // 整个窗口内容都作为ROI
    RegionOfInterest roi();

private:
    std::string m_title;        // 窗口标题
    HWND        m_handle;       // 窗口handle
    WINDOWINFO  m_window_info;  // 窗口信息

    int rect_height(RECT& rect) { return rect.bottom - rect.top; }
    int rect_width(RECT& rect) { return rect.right - rect.left; }

    // 上边框 + 标题栏的高度，转换绝对坐标的时候要加上去
    int title_and_border_height();
};

/**
 * @brief 根据标题找到对应的窗口信息
 * 
 * 标题大于title也算符合
 * 例如，title = 诛仙
 * 则能找到的窗口标题：诛仙、诛仙3
 * 但123诛仙 不符合
 * 
 * @param title 窗口标题
 * @return std::optional<Window> 
 */
std::optional<Window> find_window(PCHAR title);

};  // namespace autozhuxian

#endif