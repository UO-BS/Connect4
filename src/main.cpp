#include "Player.h"
#include "Piece.h"
#include "ConnectGame.h"
#include <string>
#include <iostream>
#include <limits>

int main()
{
    Player player1 = Player("Player1 ");
    Player player2 = Player("Player2 ");
    Player playerList[2]{player1,player2};
    ConnectGame* mainGame = new ConnectGame{playerList,8,8};
    
    mainGame->dropPiece(1, Piece(player1));
    mainGame->dropPiece(1, Piece(player1));


    for (int row=7;row>=0;row--) {
        for (int column=0;column<8;column++) {
            if ((mainGame->getBoard())[column][row]) {
                std::cout << (mainGame->getBoard())[column][row].getOwner().getName();
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