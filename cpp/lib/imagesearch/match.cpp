#include <imagesearch/match.hpp>

namespace impl {

static cv::Point match_template(const cv::Mat& source,
                                const cv::Mat& templ,
                                const cv::Mat& mask,
                                int method = cv::TM_SQDIFF)
{
    cv::Mat result;
    result.create(source.rows - templ.rows + 1, source.cols - templ.cols + 1, CV_32FC1);

    cv::matchTemplate(source, templ, result, method, mask);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1);

    cv::Point best_loc;
    if (method < 2) {
        cv::minMaxLoc(result, NULL, NULL, &best_loc, NULL);
    } else {
        cv::minMaxLoc(result, NULL, NULL, NULL, &best_loc);
    }
    return best_loc;
}

};  // namespace impl

namespace autozhuxian {

cv::Point match_template(const cv::Mat& source,
                         const cv::Mat& templ,
                         int method)
{
    return impl::match_template(source, templ, cv::Mat(), method);
}

cv::Point match_template_with_mask(const cv::Mat& source,
                                   const cv::Mat& templ,
                                   const cv::Mat& mask,
                                   int method)
{
    // TODO 替换assert
    assert(method == cv::TM_SQDIFF || method == cv::TM_CCORR_NORMED);
    return impl::match_template(source, templ, mask, method);
}

};  // namespace autozhuxian