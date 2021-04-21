#ifndef KOALAYT_MATCH_HPP_20210408
#define KOALAYT_MATCH_HPP_20210408

// std
#include <optional>
#include <filesystem>
// third party
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <fmt/core.h>
// project
#include <logger/logger.hpp>

namespace autozhuxian {

///
/// 要搜索图片的文件路径
/// ---------------------------------------------------------
/// 可能只有target，可能包含target及其对应的mask
///
class ImageSearchTarget {
public:
    // ---------------------------------------------------------
    // Constructor：只包含target
    //
    ImageSearchTarget(const char* target) : m_mask{nullptr}
    {
        check_path_valid(target);
        m_target = target;
    }

    // ---------------------------------------------------------
    // Constructor：包含target及其mask
    //
    ImageSearchTarget(const char* target, const char* mask)
    {
        check_path_valid(target);
        check_path_valid(mask);
        m_target = target;
        m_mask = mask;
    }

    // ---------------------------------------------------------
    // 判断是否包含mask
    //
    bool has_mask() { return m_mask != nullptr; }

    // ---------------------------------------------------------
    // target图片的尺寸信息
    // TODO check if target has been called!
    int width() { return m_width; }
    int height() { return m_height; }

    // ---------------------------------------------------------
    // 获取target图片的cv::Mat
    //
    cv::Mat target()
    {
        auto result = cv::imread(m_target, cv::IMREAD_UNCHANGED);
        m_width = result.cols;
        m_height = result.rows;
        return result;
    }

    // ---------------------------------------------------------
    // 获取mask图片的cv::Mat
    //
    cv::Mat mask() { return cv::imread(m_mask, cv::IMREAD_GRAYSCALE); }

private:
    const char* m_target;     // 目标图片的路径
    const char* m_mask;       // 对应mask的路径
    int         m_width{0};   // 图片的宽度
    int         m_height{0};  // 图片的高度

    // ---------------------------------------------------------
    // 验证文件路径是否合法
    //
    void check_path_valid(const char* path)
    {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error{fmt::format("file: {} does not exists\n", path)};
        }
    }
};

///
/// 图片搜索
/// ---------------------------------------------------------
/// 封装了常用的两种匹配方法
///
class Matcher : public Logger<Matcher> {
public:
    // ---------------------------------------------------------
    // Constructor：带自定义置信度
    //
    Matcher(const cv::Mat& source, double confidence)
        : m_source{source},
          m_method{cv::TM_SQDIFF_NORMED},
          m_confidence{confidence}
    {}

    // ---------------------------------------------------------
    // Constructor：默认置信度
    //
    explicit Matcher(const cv::Mat& source)
        : m_source{source},
          m_method{cv::TM_SQDIFF_NORMED},
          m_confidence{0.99}
    {}

    // ---------------------------------------------------------
    // 搜索目标图片的坐标
    // 只使用opencv中带_NORMED后缀的方法，若最佳值满足置信度，则认为找到了目标图片。
    //（暂时只支持TM_SQDIFF_NORMED方法，且不支持修改。）
    //
    std::optional<cv::Point> search(const cv::Mat& templ);

    // ---------------------------------------------------------
    // 搜索目标图片的坐标（带mask）
    // 使用TM_SQDIFF和TM_CCORR_NORMED计算两次，
    // 当两者的最佳结果相等，且TM_CCORR_NORMED的最佳值符合置信度时，
    // 才认为找到了目标图片。
    //
    std::optional<cv::Point> search_with_mask(const cv::Mat& templ, const cv::Mat& mask);

private:
    const cv::Mat& m_source;      // 待搜索源图片
    int            m_method;      // 目前只支持cv::TM_SQDIFF_NORMED
    double         m_confidence;  // 置信度
};

};  // namespace autozhuxian

#endif