#ifndef KOALAYT_TASK_HPP_20210409
#define KOALAYT_TASK_HPP_20210409

#include <vector>
#include <imagesearch/screenshot.hpp>

namespace autozhuxian {

/**
 * @brief 某个诛仙任务下的子流程
 * 
 */
struct Process {
    const char* name;      // 流程名
    RegionOfInterest roi;  // 搜索的区域
    const char* target;    // 搜索的目标图片路径
    const char* mask;      // 搜索目标的mask
    // Command* command;      // 要进行的操作
    int wait;             // 操作完成后需要等待的时间(ms)
    const char* confirm;  // 确认流程完成的图片路径
};

/**
 * @brief 诛仙任务 （包括登录，也是一种任务）
 * 
 * 每个任务都对应某个window，即在某个窗口下，根据配置好的流程，完成以下动作
 * 
 * 1. 在ROI中搜索目标图片，以获得UI的坐标（或者直接硬编码坐标位置）
 * 2. 成功：鼠标/键盘操作 -> //? 操作成功的反馈
 * 3. 失败：//? 返回上一流程？循环当前流程？直接恢复初始状态，重新再来？
 * 
 */
class Task {
private:
    std::vector<Process> m_processes;
};

};  // namespace autozhuxian

#endif