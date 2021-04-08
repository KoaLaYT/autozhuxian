#ifndef KOALAYT_MATCH_HPP_20210408
#define KOALAYT_MATCH_HPP_20210408

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace autozhuxian {

/**
 * @brief 在source图片中找到templ的位置
 * 
 * @param source 搜索的图片
 * @param templ 匹配的图片
 * @param method 算法
 * @return cv::Point 对应位置的左上角坐标（相对source图片）
 */
cv::Point match_template(const cv::Mat& source,
                         const cv::Mat& templ,
                         int method = cv::TM_SQDIFF);

/**
 * @brief 在source图片中找到templ的位置（带mask）
 * 
 * @param source 搜索的图片
 * @param templ 匹配的图片
 * @param mask 匹配图片的mask，需要和templ尺寸一致
 * @param method 算法
 * @return cv::Point 对应位置的左上角坐标（相对source图片）
 */
cv::Point match_template_with_mask(const cv::Mat& source,
                                   const cv::Mat& templ,
                                   const cv::Mat& mask,
                                   int method = cv::TM_SQDIFF);

};  // namespace autozhuxian

#endif