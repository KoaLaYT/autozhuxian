#include <gtest/gtest.h>

#include <windows.h>
#include <winuser.h>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>

#define BASE "C:\\Users\\koalayt\\Desktop\\autozhuxian\\cpp\\"
#define PATH(p) BASE p

BITMAPINFOHEADER createBitmapHeader(int width, int height);
cv::Mat captureScreenMat(HWND hwnd);

static BOOL CALLBACK EnumWindowCb(HWND hwnd, LPARAM lParam)
{
    CHAR title[1024];
    int length = GetWindowTextA(hwnd, title, sizeof(title));

    std::cout << title << '\n';

    if (!IsWindowVisible(hwnd) ||
        length == 0 ||
        std::strcmp(title, "Program Manager") == 0 ||
        std::strcmp("诛仙3", title) != 0) {
        return TRUE;
    }

    SetForegroundWindow(hwnd);
    cv::Mat screenshot = captureScreenMat(hwnd);
    cv::imwrite(PATH("screenshot.png"), screenshot);

    return TRUE;
}

TEST(windows_api, wchar)
{
    // _setmode(_fileno(stdout), _O_U16TEXT);
    EnumWindows(EnumWindowCb, NULL);
}

BITMAPINFOHEADER createBitmapHeader(int width, int height)
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

cv::Mat captureScreenMat(HWND hwnd)
{
    cv::Mat src;

    // get handles to a device context (DC)
    HDC hWindowDC = GetDC(hwnd);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);
    SetStretchBltMode(hMemoryDC, COLORONCOLOR);

    // define scale, height and width
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;

    // create mat object
    src.create(height, width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hWindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hMemoryDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hMemoryDC, 0, 0, width, height, hWindowDC, 0, 0, width, height, SRCCOPY);    //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hMemoryDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hMemoryDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return src;
}