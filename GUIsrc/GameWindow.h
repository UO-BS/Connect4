#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "BaseWindow.h"
#include "BoardWindow.h"
#include "ConnectGame.h"

class GameWindow : public BaseWindow<GameWindow>
{
private:
    BoardWindow m_boardWindow;
    HWND m_textHwnd;
    ConnectGame& m_currentGame;
public:
    PCWSTR  className() const;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void setOutputText(LPCSTR newText);

    GameWindow()=delete;
    GameWindow(ConnectGame& newGame);
};


#endif