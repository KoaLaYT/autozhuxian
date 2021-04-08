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
               hWindowDC, region.left, region.top, region.width, region.height,
               SRCCOPY);                                                                           //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hMemoryDC, hbwindow, 0, region.height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hMemoryDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return src;
}

};  // namespace autozhuxian