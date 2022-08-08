#include "Player.h"
#include "Piece.h"
#include "ConnectGame.h"
#include <string>
#include <iostream>
#include <limits>
#include <math.h>


//IMPORTANT NOTE: This is only for testing. The program is currently not usable
int main()
{
    std::string tempName1;
    std::string tempName2;
    std::cout << "Enter the name of Player 1: ";
    std::cin >> tempName1;
    std::cout << "Enter the name of Player 2: ";
    std::cin >> tempName2;
    Player player1 = Player('1', tempName1);
    Player player2 = Player('2', tempName2);
    
    Player playerList[2]{player1,player2};
    ConnectGame* mainGame = new ConnectGame{playerList,7,6};
    
    int columnNum{-1};
    while (!mainGame->gameIsOver()) {
       
        mainGame->displayBoardToConsole();

        do { 
            std::cout << "What column do you want to drop a piece?: ";
            std::cin >> columnNum;
        } while (!mainGame->dropPiece(columnNum, Piece(playerList[mainGame->getTurnNumber()%2==1])));

    }
    mainGame->displayBoardToConsole();

    std::cout <<mainGame->getWinningPlayer()->getName() <<" has won the game!\n";

    delete mainGame;
    
    std::cout << "Press Enter to exit the program ";
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); 

    return 0;
}