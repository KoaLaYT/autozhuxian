// std
#include <cstdio>
// project
#include <imagesearch/match.hpp>

namespace autozhuxian {

std::optional<cv::Point> Matcher::search(const cv::Mat& templ)
{
    cv::Mat result;
    result.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);

    cv::matchTemplate(m_source, templ, result, m_method);

    cv::Point best_loc;
    double    best_value;
    cv::minMaxLoc(result, &best_value, NULL, &best_loc, NULL);

    // TODO replace with logger
    std::printf("best value is: %.3f\n", best_value);

    // for cv::TM_SQDIFF_NORMED, best_value is between [0, 1] where 0 is the best
    if (best_value > 1 - m_confidence) {
        return std::nullopt;
    } else {
        return std::optional{best_loc};
    }
}

std::optional<cv::Point> Matcher::search_with_mask(const cv::Mat& templ, const cv::Mat& mask)
{
    cv::Mat result_m1, result_m2;
    result_m1.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);
    result_m2.create(m_source.rows - templ.rows + 1, m_source.cols - templ.cols + 1, CV_32FC1);

    cv::matchTemplate(m_source, templ, result_m1, cv::TM_SQDIFF, mask);
    cv::matchTemplate(m_source, templ, result_m2, cv::TM_CCORR_NORMED, mask);

    cv::Point best_loc_m1;
    cv::Point best_loc_m2;
    double    best_value_m2;
    cv::minMaxLoc(result_m1, NULL, NULL, &best_loc_m1, NULL);
    cv::minMaxLoc(result_m2, NULL, &best_value_m2, NULL, &best_loc_m2);

    // TODO replace with logger
    std::printf("      TM_SQDIFF: (%d, %d)\n", best_loc_m1.x, best_loc_m1.y);
    std::printf("TM_CCORR_NORMED: (%d, %d), best value: %.3f\n", best_loc_m2.x, best_loc_m2.y, best_value_m2);

    if (best_loc_m1 == best_loc_m2 && best_value_m2 >= m_confidence) {
        return std::optional{best_loc_m1};
    } else {
        return std::nullopt;
    }
}

};  // namespace autozhuxian