#include <imagesearch/screenshot.hpp>

namespace impl {

static BITMAPINFOHEADER create_bitmap_header(int width, int height)
{
    BITMAPINFOHEADER bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

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

cv::Mat screenshot_region(HWND hwnd, ScreenshotRegion region)
{
    cv::Mat src;

    // get handles to a device context (DC)
    HDC hWindowDC = GetDC(hwnd);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);
    SetStretchBltMode(hMemoryDC, COLORONCOLOR);

    // create mat object
    src.create(region.height, region.width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hWindowDC, region.width, region.height);
    BITMAPINFOHEADER bi = impl::create_bitmap_header(region.width, region.height);

    // use the previously created device context with the bitmap
    SelectObject(hMemoryDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hMemoryDC, 0, 0, region.width, region.height,
               hWindowDC, region.top, region.left, region.width, region.height, SRCCOPY);          //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hMemoryDC, hbwindow, 0, region.height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hMemoryDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return src;
}

cv::Point match_template(const cv::Mat& source,
                         const cv::Mat& templ,
                         int method = cv::TM_SQDIFF)
{
    return impl::match_template(source, templ, cv::Mat(), method);
}

cv::Point match_template_with_mask(const cv::Mat& source,
                                   const cv::Mat& templ,
                                   const cv::Mat& mask,
                                   int method = cv::TM_SQDIFF)
{
    // TODO 替换assert
    assert(method == cv::TM_SQDIFF || method == cv::TM_CCORR_NORMED);
    return impl::match_template(source, templ, mask, method);
}

};  // namespace autozhuxian