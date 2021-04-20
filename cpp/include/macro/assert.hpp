#ifndef KOALAYT_ASSERT_HPP_20210420
#define KOALAYT_ASSERT_HPP_20210420

#include <utility>
#include <cstdlib>

#ifndef NDEBUG
#include <fmt/core.h>
#define AUTOZHUXIAN_ASSERT(expr, msg) internal_assert(#expr, expr, __FILE__, __LINE__, msg)
#else
#define AUTOZHUXIAN_ASSERT(expr, msg) (void)0
#endif

/// Assert with simple information (expectation, file location, etc)
template <typename T>
static void internal_assert(const char* expr, bool cond, const char* file, int line, T&& msg)
{
    if (!cond) {
        fmt::print(
            "\n"
            "{:>15}: {}\n"
            "{:>15}: {}\n"
            "{:>15}: {}:{}\n",
            "Assert failed", std::forward<T>(msg),
            "Expected", expr,
            "Location", file, line);
        std::abort();
    }
}

#endif