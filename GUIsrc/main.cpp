#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    Player player1 = Player('1');
    Player player2 = Player('2');
    Player playerList[2]{player1,player2};
    ConnectGame mainGame{playerList,7,6};


    GameWindow mainWin{mainGame}; //GameWindows should not outlive the game

    if (!mainWin.create(L"My Window", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(mainWin.getHandle(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
