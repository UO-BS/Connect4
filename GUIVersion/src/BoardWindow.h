#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "BaseWindow.h"

class BoardWindow : public BaseWindow<BoardWindow>
{
private:
    LPCWSTR m_imagePath{L"../resources/boardV1.bmp"};
    int m_srcHeight{700};
    int m_srcWidth{800};
public:
    PCWSTR  className() const;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif