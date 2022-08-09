#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include <windows.h>

template <class DERIVED_TYPE>
class BaseWindow
{
private:
protected:

    HWND m_hwnd;
    virtual PCWSTR className() const =0;
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) =0;

public:
    
    BaseWindow(): m_hwnd{NULL} {}
    HWND getHandle() const {return m_hwnd;}

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *derivedThis = nullptr;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* derivedCreate = (CREATESTRUCT*)lParam;
            derivedThis = (DERIVED_TYPE*)derivedCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)derivedThis);

            derivedThis->m_hwnd = hwnd;
        }
        else {
            derivedThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        
        if (derivedThis) {
            return derivedThis->handleMessage(uMsg, wParam, lParam);
        }
        else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    bool create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0)
    {
        WNDCLASS wc = {};

        wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;  //WindowProc defines the behavior of the window
        wc.hInstance     = GetModuleHandle(NULL);   //"hInstance is the handle of the application instance"
        wc.lpszClassName = className();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(dwExStyle, className(), lpWindowName, dwStyle, x, y,nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

        return (m_hwnd ? true : false);
    }

};

#endif