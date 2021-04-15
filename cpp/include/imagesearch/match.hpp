#ifndef KOALAYT_MATCH_HPP_20210408
#define KOALAYT_MATCH_HPP_20210408

// std
#include <optional>
#include <filesystem>
// third party
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <fmt/core.h>

namespace autozhuxian {

/**
 * @brief 图片搜索的文件路径，可能包含mask文件
 * 
 */
class ImageSearchTarget {
public:
    ImageSearchTarget(const char* target) : m_mask{nullptr}
    {
        check_path_valid(target);
        m_target = target;
    }

    ImageSearchTarget(const char* target, const char* mask)
    {
        check_path_valid(target);
        check_path_valid(mask);
        m_target = target;
        m_mask = mask;
    }

    bool has_mask() { return m_mask != nullptr; }

    cv::Mat target() { return cv::imread(m_target, cv::IMREAD_UNCHANGED); }
    cv::Mat mask() { return cv::imread(m_mask, cv::IMREAD_GRAYSCALE); }

private:
    const char* m_target;
    const char* m_mask;

    // 验证路径合法
    void check_path_valid(const char* path)
    {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error{fmt::format("file: {} does not exists\n", path)};
        }
    }
};

/**
 * @brief 图片搜索
 * 
 */
class Matcher {
public:
    Matcher(const cv::Mat& source, double confidence)
        : m_source{source},
          m_method{cv::TM_SQDIFF_NORMED},
          m_confidence{confidence}
    {}

    explicit Matcher(const cv::Mat& source)
        : m_source{source},
          m_method{cv::TM_SQDIFF_NORMED},
          m_confidence{0.99}
    {}

    /**
     * @brief 搜索目标图片的坐标。
     * 
     * 只使用opencv中带_NORMED后缀的方法，若最佳值满足置信度，则认为找到了目标图片。
     * （暂时只支持TM_SQDIFF_NORMED方法，且不支持修改。）
     * 
     * @param templ 目标图片
     * @return std::optional<cv::Point> 对应位置的左上角坐标（相对source图片）
     */
    std::optional<cv::Point> search(const cv::Mat& templ);

    /**
     * @brief 搜索目标图片的坐标（带mask）。
     * 
     * 使用TM_SQDIFF和TM_CCORR_NORMED计算两次，
     * 当两者的最佳结果相等，且TM_CCORR_NORMED的最佳值符合置信度时，
     * 才认为找到了目标图片。
     * 
     * @param templ 目标图片
     * @param mask 目标图片的mask，需要和templ尺寸一致
     * @return std::optional<cv::Point> 对应位置的左上角坐标（相对source图片）
     */
    std::optional<cv::Point> search_with_mask(const cv::Mat& templ, const cv::Mat& mask);

private:
    const cv::Mat& m_source;      // 待搜索源图片
    int            m_method;      // current only support cv::TM_SQDIFF_NORMED
    double         m_confidence;  // 置信度
};

};  // namespace autozhuxian

#endif