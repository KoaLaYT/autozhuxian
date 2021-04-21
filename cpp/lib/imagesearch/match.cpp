// std
#include <cstdio>
// project
#include <imagesearch/match.hpp>

namespace autozhuxian {

///
/// 图片搜索
/// ---------------------------------------------------------
/// 搜索目标图片的坐标
///
std::optional<cv::Point> Matcher::search(const cv::Mat& templ)
{
    // 初始化匹配结果矩阵
    // ---------------------------------------------------------
    cv::Mat result;
    result.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);

    // 开始搜索匹配
    // ---------------------------------------------------------
    cv::matchTemplate(m_source, templ, result, m_method);

    // 找到结果中的最佳值
    // ---------------------------------------------------------
    cv::Point best_loc;
    double    best_value;
    cv::minMaxLoc(result, &best_value, NULL, &best_loc, NULL);

    info(">>>图片搜索最佳值: {:.3f}，置信度：{:.3f}", best_value, m_confidence);

    // for cv::TM_SQDIFF_NORMED,
    // best_value is between [0, 1] where 0 is the best
    // ---------------------------------------------------------
    if (best_value > 1 - m_confidence) {
        return std::nullopt;
    } else {
        return std::optional{best_loc};
    }
}

///
/// 图片搜索
/// ---------------------------------------------------------
/// 搜索目标图片的坐标（带mask）
///
std::optional<cv::Point> Matcher::search_with_mask(const cv::Mat& templ, const cv::Mat& mask)
{
    // 初始化结果矩阵
    // ---------------------------------------------------------
    cv::Mat result_m1, result_m2;
    result_m1.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);
    result_m2.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);

    // 用两种方法各搜索一次
    // 因为测试下来，发现TM_CCORR_NORMED会有false positive，而TM_SQDIFF又无法确定它的置信度
    // 所以两个方法都用一次，减少错误匹配的可能性（效率不是问题，这个项目里没有要求）
    // ---------------------------------------------------------
    cv::matchTemplate(m_source, templ, result_m1, cv::TM_SQDIFF, mask);
    cv::matchTemplate(m_source, templ, result_m2, cv::TM_CCORR_NORMED, mask);

    // 找到各自的最佳值
    // ---------------------------------------------------------
    cv::Point best_loc_m1;
    cv::Point best_loc_m2;
    double    best_value_m2;
    cv::minMaxLoc(result_m1, NULL, NULL, &best_loc_m1, NULL);
    cv::minMaxLoc(result_m2, NULL, &best_value_m2, NULL, &best_loc_m2);

    info(">>>      TM_SQDIFF: ({}, {})", best_loc_m1.x, best_loc_m1.y);
    info(">>>TM_CCORR_NORMED: ({}, {}), 最佳值: {:.3f}，置信度：{:.3f}", best_loc_m2.x, best_loc_m2.y, best_value_m2, m_confidence);

    // 当且仅当两者位置相同，且满足置信度时，才认为找到了目标图片
    // ---------------------------------------------------------
    if (best_loc_m1 == best_loc_m2 && best_value_m2 >= m_confidence) {
        return std::optional{best_loc_m1};
    } else {
        return std::nullopt;
    }
}

};  // namespace autozhuxian