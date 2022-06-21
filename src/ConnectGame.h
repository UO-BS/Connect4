#ifndef CONNECT_GAME_H
#define CONNECT_GAME_H

#include "Piece.h"
#include "Player.h"
#include "Position.h"

class ConnectGame
{
private:
    Position** m_board;
    const Player (&m_playerList)[2];
    const int m_columnNum;
    const int m_rowNum;
    
    void placePiece(int column, int row, const Piece& newPiece);

    Player checkForWin() const;
public:
    //remove default constructor
    ConnectGame()=delete;
    
    ConnectGame(const Player (&players)[2]);

    //Checker Games cannot be copied
    ConnectGame(const ConnectGame& original)=delete;

    //Returns a copy of the board
    Position** const getBoard() const;    

    void dropPiece(int column, const Piece& newPiece);
};

#endif