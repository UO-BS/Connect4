#include "Player.h"
#include "Piece.h"
#include "ConnectGame.h"
#include <string>
#include <iostream>
#include <limits>

int main()
{
    Player playerList[2]{Player("Player1"),Player("Player2")};
    
    ConnectGame* mainGame = new ConnectGame{playerList};
    
    mainGame->dropPiece(1, Piece(Player("TESTER")));

    for (int i=0;i<8;i++) {
        for (int column=0;column<8;column++) {
            if ((mainGame->getBoard())[column][i].hasPiece()) {
                std::cout << (mainGame->getBoard())[column][i].getPiece().getOwner().getName();
            } else {
                std::cout << "NoPiece ";
            }
        }
        std::cout << "\n";
    }

    delete mainGame;
    
    std::cout << "Press Enter to exit the program ";
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); 

    return 0;
}