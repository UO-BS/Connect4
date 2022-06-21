#include "ConnectGame.h"

ConnectGame::ConnectGame(const Player (&players)[2]) : m_board{} , m_playerList{players}, m_columnNum{8} , m_rowNum{8}
{
    m_board = new Position*[m_columnNum];
    for (int i=0;i<m_columnNum;i++) {
        m_board[i] = new Position[m_rowNum];
    }
}

Position** const ConnectGame::getBoard() const
{
    return m_board;
}

void ConnectGame::placePiece(int column, int row, const Piece& newPiece)
{
    m_board[column][row].setPiece(newPiece);
}

void ConnectGame::dropPiece(int column, const Piece& newPiece)
{
    for (int i=0;i<m_rowNum;i++) {
        if (m_board[column][i].hasPiece() && i==0) {
            //THE COLUMN IS FULL, CURRENTLY NOT HANDLED
            return;
        }
        if (m_board[column][i].hasPiece()) {
            ConnectGame::placePiece(column,i-1,newPiece);
            return;
        }
        if (i==7) {
            ConnectGame::placePiece(column,i,newPiece);
            return;
        }
    }
    
}

Player ConnectGame::checkForWin() const
{
    for (int column=0;column<m_columnNum;column++) {
        for (int row=0;row<m_rowNum;row++) {
            if (m_board[column][row].hasPiece())
            {
                //CHECK FOR A CONNECT 4
            }
        }
    }
}