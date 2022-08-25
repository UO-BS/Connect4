#ifndef UNICODE
#define UNICODE
#endif 

#include "FadingTextWindow.h"
#include <iostream>

//Creating a way to convert std::strings to std::wstrings
#include <locale>
#include <codecvt>
#include <string>
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

FadingTextWindow::FadingTextWindow(int rows,bool fadeDownwards, COLORREF textColor, COLORREF backgroundColor) : m_rows{rows}, m_backgroundColor{backgroundColor}, m_textColor{textColor}, m_fadeDownwards{fadeDownwards}
{
    m_staticControls.resize(m_rows);
}

PCWSTR FadingTextWindow::className() const{return L"FadingTextWindow";}
LRESULT FadingTextWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
        LRESULT lRes = DefWindowProc(m_hwnd, uMsg, wParam, lParam);

        for (int i=0;i<m_rows;i++) {
            m_staticControls[i] = std::pair{CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER, 0,0,0,0,m_hwnd,(HMENU)(INT_PTR)i,GetModuleHandle(NULL),NULL),""};
        }

        return lRes;
        }

    case WM_SIZE:
        {
            int height = HIWORD(lParam)/m_rows;
            for (int i=0;i<m_rows;i++) {
                int index = (m_fadeDownwards)?i:m_rows-i-1;
                SetWindowPos(m_staticControls[i].first,NULL,0,height*index,LOWORD(lParam),height,SWP_NOZORDER);
            }
            RedrawWindow(m_hwnd,NULL,NULL, RDW_INVALIDATE);
        }
        return 0;
    
    case WM_CTLCOLORSTATIC:
        {
            if (m_tempBrush!=NULL) {
                DeleteObject(m_tempBrush);
                m_tempBrush = NULL;
            }
            
            HDC staticHDC = (HDC)wParam;

            COLORREF desiredColor;
            for (int i=0;i<m_staticControls.size();i++) {
                if (m_staticControls[i].first==(HWND)lParam) {
                    int r = GetRValue(m_textColor) + i*(GetRValue(m_backgroundColor)-GetRValue(m_textColor))/(m_rows+1);
                    int g = GetGValue(m_textColor) + i*(GetGValue(m_backgroundColor)-GetGValue(m_textColor))/(m_rows+1);
                    int b = GetBValue(m_textColor) + i*(GetBValue(m_backgroundColor)-GetBValue(m_textColor))/(m_rows+1);
                    desiredColor = RGB(r,g,b);
                } 
            }

            SetTextColor(staticHDC, desiredColor);
            SetBkColor(staticHDC, m_backgroundColor);
            m_tempBrush = CreateSolidBrush(m_backgroundColor);

            return (LRESULT)m_tempBrush;
        }
    

    case WM_CLOSE:
        {
            if (m_tempBrush!=NULL) {
                DeleteObject(m_tempBrush);
                m_tempBrush = NULL;
            }
            LRESULT lRes = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
            return lRes;
        }

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

void FadingTextWindow::addText(const std::string& newText)
{
    for (int i=m_staticControls.size()-1;i>0;i--) {
        SetWindowText(m_staticControls[i].first,converter.from_bytes(m_staticControls[i-1].second).c_str());
        m_staticControls[i].second = m_staticControls[i-1].second;
    }
    m_staticControls[0].second = newText;
    SetWindowText(m_staticControls[0].first,converter.from_bytes(m_staticControls[0].second).c_str());
}
