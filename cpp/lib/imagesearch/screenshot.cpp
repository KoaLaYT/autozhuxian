// project
#include <imagesearch/screenshot.hpp>

namespace impl {

///
/// 初始化一个BITMAPINFOHEADER
/// ---------------------------------------------------------
///
///
static BITMAPINFOHEADER create_bitmap_header(int width, int height)
{
    BITMAPINFOHEADER bi;

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

};  // namespace impl

// ------------------------------------------------------------------------------- //
// ---------------------------------DIVIDER--------------------------------------- //
// ------------------------------------------------------------------------------- //

namespace autozhuxian {

///
/// ROI 窗口的某一区域
/// ---------------------------------------------------------
/// 一个特殊的值，代表整个屏幕都是ROI
///
RegionOfInterest RegionOfInterest::whole = RegionOfInterest{0, 0, 0, 0};

///
/// 图像搜索
/// ---------------------------------------------------------
/// 对窗口的某一区域截图
/// 主要参考：https://superkogito.github.io/blog/CaptureScreenUsingGdiplus.html
///
cv::Mat screenshot_region(HWND hwnd, RegionOfInterest roi)
{
    cv::Mat src;

    // get handles to a device context (DC)
    // ---------------------------------------------------------
    HDC hWindowDC = GetDC(hwnd);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);
    SetStretchBltMode(hMemoryDC, COLORONCOLOR);

    // create mat object
    // ---------------------------------------------------------
    src.create(roi.height, roi.width, CV_8UC4);

    // create a bitmap
    // ---------------------------------------------------------
    HBITMAP          hbwindow = CreateCompatibleBitmap(hWindowDC, roi.width, roi.height);
    BITMAPINFOHEADER bi = impl::create_bitmap_header(roi.width, roi.height);

    // use the previously created device context with the bitmap
    // ---------------------------------------------------------
    SelectObject(hMemoryDC, hbwindow);

    // copy from the window device context to the bitmap device context
    // ---------------------------------------------------------
    StretchBlt(hMemoryDC, 0, 0, roi.width, roi.height,
               hWindowDC, roi.x, roi.y, roi.width, roi.height,
               SRCCOPY);                                                                        //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hMemoryDC, hbwindow, 0, roi.height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hMemoryDC to hbwindow

    // avoid memory leak
    // ---------------------------------------------------------
    DeleteObject(hbwindow);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return src;
}

};  // namespace autozhuxian