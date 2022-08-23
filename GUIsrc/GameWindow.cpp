#ifndef UNICODE
#define UNICODE
#endif 

#include "GameWindow.h"
#include <iostream>

GameWindow::GameWindow(ConnectGame& newGame) : BaseWindow{},m_boardWindow{newGame}, m_currentGame{newGame}{}

PCWSTR  GameWindow::className() const { return L"MainWindow"; }
LRESULT GameWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
        LRESULT lRes = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        
        m_textHwnd = CreateWindowEx(0, L"STATIC", L"MyLabel", WS_CHILD | WS_VISIBLE, 1000,0,100,100,m_hwnd,(HMENU)2,GetModuleHandle(NULL),NULL);
        //Having different dimensions than the myCircleButton object will crop it
        m_boardWindow.create(L"MyBoardDisplay",WS_CHILD | WS_VISIBLE,0,0,0,1000,1000,m_hwnd,(HMENU)1);

        return lRes;
        }
    case WM_SIZE:
        {
            RECT windowRect;
            int windowWidth, windowHeight;
            GetWindowRect(m_hwnd, &windowRect);
            windowWidth = windowRect.right - windowRect.left;
            windowHeight = windowRect.bottom - windowRect.top;

            bool result;

            result = MoveWindow(m_textHwnd,windowWidth*(2.0/3.0)+1,0,windowWidth*(1.0/3.0),windowHeight,TRUE);
            if (!result) {
                return 1;
            }
            result = MoveWindow(m_boardWindow.getHandle(),0,0,windowWidth*(2.0/3.0),windowHeight,TRUE);
            if (!result) {
                return 1;
            }
        }
        return 0;

    case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
            minMaxInfo->ptMinTrackSize.x = 1000;
            minMaxInfo->ptMinTrackSize.y = 500;
        }
        return 0;

    case WM_COMMAND:
        switch(LOWORD(wParam)){
            case 1: //If message was from 1 control
                std::cout << "1";
                break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0); //Exits the message while loop, ending the program
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            
            //Color the background Black
            HBRUSH brush = CreateSolidBrush(0x00FFFFFF);
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);
            
            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

void GameWindow::setOutputText(LPCSTR newText)
{
    SetWindowTextA(m_textHwnd,newText);
}