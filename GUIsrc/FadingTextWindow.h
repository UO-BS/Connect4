#ifndef FADING_TEXT_H
#define FADING_TEXT_H

#include "BaseWindow.h"
#include <vector>
#include <string>
#include <utility>

class FadingTextWindow : public BaseWindow<FadingTextWindow>
{
public:
    PCWSTR  className() const;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    FadingTextWindow(int rows, bool fadeDownwards = true,COLORREF textColor = 0x00000000 ,COLORREF backgroundColor = 0x00FFFFFF);
    FadingTextWindow()=delete;

    void addText(const std::string& newText);
    
private:
    int m_rows; //Use vector size instead?
    COLORREF m_backgroundColor;
    COLORREF m_textColor;

    bool m_fadeDownwards;

    std::vector<std::pair<HWND,std::string>> m_staticControls;

    HBRUSH m_tempBrush;
};

#endif