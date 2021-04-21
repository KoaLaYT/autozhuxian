#ifndef KOALAYT_LOGGER_HPP_20210420
#define KOALAYT_LOGGER_HPP_20210420

// std
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <vector>
// third party
#include <spdlog/spdlog.h>
// #include <spdlog/async.h> // win10 下程序无法退出, spdlog::shutdown也没用
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#define LOG_BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\logs\\"
#define LOG_PATH(p) LOG_BASE p

namespace impl {

template <typename T>
static std::shared_ptr<spdlog::logger> create_logger()
{
    // release版本才需要保存日志到文件，debug下只输出到console
    // ---------------------------------------------------------
#ifdef NDEBUG
    static auto shared_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_st>(LOG_PATH("log.txt"), 0, 30);
#endif

    std::vector<spdlog::sink_ptr> sinks{
        std::make_shared<spdlog::sinks::stdout_color_sink_st>(),
#ifdef NDEBUG
        shared_file_sink,
#endif
    };

// demangle type, only working in gcc
// https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
// ---------------------------------------------------------
#ifdef _MSC_VER
    auto logger = std::make_shared<spdlog::logger>(typeid(T).name(), sinks.begin(), sinks.end());
#elif
    int   status;
    char* name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    auto  logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    delete name;
#endif

#ifndef NDEBUG
    logger->set_level(spdlog::level::debug);
#endif
    return logger;
}

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

#define STR_LOG(level)        \
    template <typename S>     \
    void level(const S& msg)  \
    {                         \
        s_logger->level(msg); \
        s_logger->flush();    \
    }

#define FMT_LOG(level)                                     \
    template <typename FormatString, typename... Args>     \
    void level(const FormatString& fmt, Args&&... args)    \
    {                                                      \
        s_logger->level(fmt, std::forward<Args>(args)...); \
        s_logger->flush();                                 \
    }

#define LOG(level) \
    STR_LOG(level) \
    FMT_LOG(level)

namespace autozhuxian {

///
/// 用于日志打印的类
/// ---------------------------------------------------------
/// 继承这个类即自动实现了打印日志的功能
///
template <typename T>
class Logger {
protected:
    // ---------------------------------------------------------
    // 不同等级的打印
    //
    LOG(trace)
    LOG(debug)
    LOG(info)
    LOG(warn)
    LOG(error)
    LOG(critical)

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};
template <typename T>
std::shared_ptr<spdlog::logger> Logger<T>::s_logger = impl::create_logger<T>();

};  // namespace autozhuxian

#undef STR_LOG
#undef FMT_LOG
#undef LOG

#endif