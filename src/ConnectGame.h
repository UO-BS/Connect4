#ifndef CONNECT_GAME_H
#define CONNECT_GAME_H

#include "Piece.h"
#include "Player.h"

class ConnectGame
{
private:
    //Member Variables
    Piece** m_board; //Position 0,0 is at the bottom left of the board
    const Player (&m_playerList)[2];
    const int m_columnNum;
    const int m_rowNum;
    int m_turnCount;
    
    const Player* m_winningPlayer;
    bool m_gameOver;
    
    //Helper function to place a piece
    void placePiece(int column, int row, const Piece& newPiece);

    //Functions to check if a player has won
    bool checkVerticalWin(int column, int row) const;
    bool checkHorizontalWin(int column, int row) const;
    bool checkUpDiagonalWin(int column, int row) const;
    bool checkDownDiagonalWin(int column, int row) const;
    bool checkTie() const;

    void checkForWin();

    //Functions to cycle game turns
    void cycleTurn();
    const Player& getCurrentTurn() const;

public:
    //remove default constructor
    ConnectGame()=delete;
    
    ConnectGame(const Player (&players)[2], int columnNumber , int rowNumber);
    
    //Games cannot be copied
    ConnectGame(const ConnectGame& original)=delete;

    ~ConnectGame();

    //Returns a copy of the board
    Piece ** getBoard() const;    

    bool dropPiece(int column, const Piece& newPiece);

    const int getRowNumber() const;
    const int getColumnNumber() const;

    bool gameIsOver() const;
    const Player* getWinningPlayer();
    
    int getTurnNumber() const;
    
    void displayBoardToConsole() const;
};

#endif