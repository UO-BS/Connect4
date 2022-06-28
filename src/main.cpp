#include "Player.h"
#include "Piece.h"
#include "ConnectGame.h"
#include "BotUser.h"
#include <string>
#include <iostream>
#include <limits>
#include <math.h>


//IMPORTANT NOTE: This is only for testing. The program is currently not usable
int main()
{
    Player player1 = Player("Player1 ");
    BotUser player2 = BotUser("weight.txt");
    
    Player playerList[2]{player1,player2};
    ConnectGame* mainGame = new ConnectGame{playerList,8,8};
    

    int columnNum{};
    int playerNum{0};
    while (!mainGame->gameIsOver()) {
        playerNum++;
        if (playerNum%2==1) {
            do {
                std::cout << "column: ";
                std::cin >> columnNum;
            } while (!mainGame->dropPiece(columnNum, Piece(player1)));
        } else {
            std::cout << mainGame->dropPiece(player2.chooseColumn(mainGame->getBoard()), Piece(player2)); //problem line
        }

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
    }

    player2.train(false);

    delete mainGame;
    
    std::cout << "Press Enter to exit the program ";
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); 

    return 0;
}