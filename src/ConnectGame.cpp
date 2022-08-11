#include "ConnectGame.h"
#include <iostream>


ConnectGame::ConnectGame(const Player (&players)[2], int columnNumber, int rowNumber) : 
    m_board{} , m_playerList{players}, m_columnNum{columnNumber} , m_rowNum{rowNumber}, m_turnCount{0}, m_winningPlayer{nullptr}, m_gameOver{false}
{
    m_board = new Piece*[m_columnNum];
    for (int i=0;i<m_columnNum;i++) {
        m_board[i] = new Piece[m_rowNum];
    }
}

ConnectGame::~ConnectGame() 
{
   for (int i=0;i<m_columnNum;i++) {
        delete[] m_board[i];
   }
   delete[] m_board;
}

Piece** ConnectGame::getBoard() const
{
    return m_board;
}

bool ConnectGame::dropPiece(int column, const Piece& newPiece)
{
    if (m_gameOver) {
        return false;
    }
    if (column >= m_columnNum || column <= 0) {
        return false;
    }
    column--; //User enters the interval [1,columnnumber], array uses interval [0,columnnumber-1]
    if (newPiece.getOwner() != getCurrentTurn()) {
        return false;
    }
    if (m_board[column][m_rowNum-1].isValidPiece()) {
        //THE COLUMN IS FULL, CURRENTLY NOT HANDLED
        return false;
    }
    for (int i=m_rowNum-1;i>=0;i--) {
        if (m_board[column][i].isValidPiece()) {
            m_board[column][i+1] = newPiece;
            break;
        }
        if (i==0) {
            m_board[column][i] = newPiece;
            break;
        }
    }
    checkForWin();
    cycleTurn();
    return true;
    
}

bool ConnectGame::checkVerticalWin(int column, int row) const
{
    int connectCount{1};
    int moveCounter{1};

    while (true) {
        if (row+moveCounter >= m_rowNum) {
            break;
        }
        else if (!m_board[column][row+moveCounter]) {
            break;
        }
        else if (m_board[column][row+moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 
    
    moveCounter=1;
    
    while (true) {
        if (row-moveCounter < 0) {
            break;
        }
        else if (!m_board[column][row-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[column][row-moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 

    if (connectCount>=4) {return true;} else {return false;}
  
}

bool ConnectGame::checkHorizontalWin(int column, int row) const
{
    int connectCount{1};
    int moveCounter{1};

    while (true) {
        if (column+moveCounter >= m_columnNum) {
            break;
        }
        else if (!m_board[column+moveCounter][row].isValidPiece()) {
            break;
        }
        else if (m_board[column+moveCounter][row].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 
    
    moveCounter=1;
    
    while (true) {
        if (column-moveCounter < 0) {
            break;
        }
        else if (!m_board[column-moveCounter][row].isValidPiece()) {
            break;
        }
        else if (m_board[column-moveCounter][row].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 

    if (connectCount>=4) {return true;} else {return false;}
}

bool ConnectGame::checkUpDiagonalWin(int column, int row) const
{
    int connectCount{1};
    int moveCounter{1};

    while (true) {
        if (column+moveCounter >= m_columnNum || row+moveCounter >= m_rowNum) {
            break;
        }
        else if (!m_board[column+moveCounter][row+moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[column+moveCounter][row+moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 
    
    moveCounter=1;
    
    while (true) {
        if (column-moveCounter < 0 || row-moveCounter < 0) {
            break;
        }
        else if (!m_board[column-moveCounter][row-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[column-moveCounter][row-moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 

    if (connectCount>=4) {return true;} else {return false;}
}

bool ConnectGame::checkDownDiagonalWin(int column, int row) const
{
    int connectCount{1};
    int moveCounter{1};

    while (true) {
        if (column+moveCounter >= m_columnNum || row-moveCounter < 0) {
            break;
        }
        else if (!m_board[column+moveCounter][row-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[column+moveCounter][row-moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 
    
    moveCounter=1;
    
    while (true) {
        if (column-moveCounter < 0 || row+moveCounter >= m_rowNum) {
            break;
        }
        else if (!m_board[column-moveCounter][row+moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[column-moveCounter][row+moveCounter].getOwner() != m_board[column][row].getOwner()) {
            break;
        }
        moveCounter++;
        connectCount++;
    } 

    if (connectCount>=4) {return true;} else {return false;}
}

bool ConnectGame::checkTie() const
{
    for (int column=0;column<m_columnNum;column++) {
        if (!m_board[column][m_rowNum-1].isValidPiece()) {
            return false;
        }
    }
    return true;
}

void ConnectGame::checkForWin()
{
    if (checkTie()){
        m_gameOver =true;
        return;
    }

    for (int column=0;column<m_columnNum;column++) {
        for (int row=0;row<m_rowNum;row++) {
            if (m_board[column][row].isValidPiece())
            {

                if (checkVerticalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[column][row].getOwner();
                    return;
                }

                if (checkHorizontalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[column][row].getOwner();
                    return;
                }

                if (checkUpDiagonalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[column][row].getOwner();
                    return;
                }

                if (checkDownDiagonalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[column][row].getOwner();
                    return;
                }
            }
        }
    }
    return;
}

const int ConnectGame::getRowNumber() const {return m_rowNum;}
const int ConnectGame::getColumnNumber() const {return m_columnNum;}

void ConnectGame::cycleTurn()
{
    m_turnCount++;
}

const Player& ConnectGame::getCurrentTurn() const
{
    return m_playerList[m_turnCount%2];
}

bool ConnectGame::gameIsOver() const
{
    return m_gameOver;
}

const Player* ConnectGame::getWinningPlayer()
{
    return m_winningPlayer;
}

int ConnectGame::getTurnNumber() const
{
    return m_turnCount;
}

void ConnectGame::displayBoardToConsole() const
{
    for (int row=7;row>=0;row--) {
            for (int column=0;column<8;column++) {
                if ((getBoard())[column][row]) {
                    std::cout <<" "<< (getBoard())[column][row].getOwner().getSymbol()<<" ";
                } else {
                    std::cout << " - ";
                }
            }
            std::cout << "\n";
    }
}

const Player* ConnectGame::getPlayerList() const
{
    return m_playerList;
}