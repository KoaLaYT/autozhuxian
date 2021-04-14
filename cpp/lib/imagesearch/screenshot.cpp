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

RegionOfInterest RegionOfInterest::from_rect(RECT rect)
{
    return RegionOfInterest{0, 0,
                            rect.right - rect.left,
                            rect.bottom - rect.top};
}

RegionOfInterest RegionOfInterest::from_hwnd(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    return RegionOfInterest::from_rect(rect);
}

cv::Mat screenshot_region(HWND hwnd, RegionOfInterest roi)
{
    cv::Mat src;

    // get handles to a device context (DC)
    HDC hWindowDC = GetDC(hwnd);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);
    SetStretchBltMode(hMemoryDC, COLORONCOLOR);

    // create mat object
    src.create(roi.height, roi.width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hWindowDC, roi.width, roi.height);
    BITMAPINFOHEADER bi = impl::create_bitmap_header(roi.width, roi.height);

    // use the previously created device context with the bitmap
    SelectObject(hMemoryDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hMemoryDC, 0, 0, roi.width, roi.height,
               hWindowDC, roi.x, roi.y, roi.width, roi.height,
               SRCCOPY);                                                                        //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hMemoryDC, hbwindow, 0, roi.height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hMemoryDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return src;
}

};  // namespace autozhuxian