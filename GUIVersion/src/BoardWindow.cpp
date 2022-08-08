#ifndef UNICODE
#define UNICODE
#endif 

#include "BoardWindow.h"
#include <iostream>

PCWSTR  BoardWindow::className() const { return L"BoardWindow"; }
LRESULT BoardWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
        LRESULT lRes = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        
        return lRes;
        }

    case WM_DESTROY:
        PostQuitMessage(0); //Exits the message while loop, ending the program
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            
            HBITMAP bmp = (HBITMAP) LoadImage(NULL,m_imagePath,IMAGE_BITMAP,m_srcWidth,m_srcHeight,LR_LOADFROMFILE);
            if (!bmp) {
                std::cout << "Error Loading .bmp file: " << GetLastError() << "\n";
                std::cout << "Image path: " << m_imagePath << "\n";
            }
            HDC tempDC = CreateCompatibleDC(hdc);
            HGDIOBJ replacedOBJ = SelectObject(tempDC, bmp);
            BitBlt(hdc,0,100,m_srcWidth,m_srcHeight,tempDC,0,0,SRCCOPY);
            //StretchBlt(hdc,0,0,100,75,tempDC,0,0,m_srcWidth,m_srcHeight,SRCCOPY);
            SelectObject(tempDC, replacedOBJ);
            DeleteObject(bmp);
            DeleteDC(tempDC);

            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}