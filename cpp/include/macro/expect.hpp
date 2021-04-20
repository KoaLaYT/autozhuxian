#ifndef KOALAYT_EXPECT_HPP_20210420
#define KOALAYT_EXPECT_HPP_20210420

#include <stdexcept>

#define AUTOZHUXIAN_EXPECT(expr, msg) internal_expect(expr, __FILE__, __LINE__, msg)

namespace autozhuxian {

///
/// Simple wrapped error
/// ---------------------------------------------------------
///
///
class ExpectError : public std::runtime_error {
public:
    explicit ExpectError(const char* err) : std::runtime_error{err} {}

    explicit ExpectError(const std::string& err) : std::runtime_error{err} {}

    explicit ExpectError(std::string&& err) : std::runtime_error{err} {}
};

}  // namespace autozhuxian

///
/// Expect
/// ---------------------------------------------------------
/// With simple information (expectation, file location, etc)
///
template <typename T>
static void internal_expect(bool cond, const char* file, int line, T&& msg)
{
    if (!cond) {
        throw autozhuxian::ExpectError{
            fmt::format("Expect `{}` failed at {}:{}",
                        std::forward<T>(msg),
                        file, line)};
    }
}

#endif