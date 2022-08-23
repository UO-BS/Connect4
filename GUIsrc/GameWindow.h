#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "BaseWindow.h"
#include "BoardWindow.h"
#include "ConnectGame.h"
#include "FadingTextWindow.h"

class GameWindow : public BaseWindow<GameWindow>
{
private:
    BoardWindow m_boardWindow;
    FadingTextWindow m_textWindow;
    
    ConnectGame& m_currentGame;
public:
    PCWSTR  className() const;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    GameWindow()=delete;
    GameWindow(ConnectGame& newGame);
};


#endif