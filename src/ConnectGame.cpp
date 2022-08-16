#include "ConnectGame.h"
#include <iostream>


ConnectGame::ConnectGame(const Player (&players)[2], int columnNumber, int rowNumber) : 
    m_board{} , m_playerList{players}, m_columnNum{columnNumber} , m_rowNum{rowNumber}, m_turnCount{0}, m_winningPlayer{nullptr}, m_gameOver{false}
{
    m_board = new Piece*[m_rowNum];
    for (int i=0;i<m_rowNum;i++) {
        m_board[i] = new Piece[m_columnNum];
    }
}

ConnectGame::~ConnectGame() 
{
   for (int i=0;i<m_rowNum;i++) {
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
    //Cannot drop piece if game is over
    if (m_gameOver) {
        return false;
    }
    //Cannot drop piece outside of board
    if (column > m_columnNum || column <= 0) {
        return false;
    }
    column--; //User enters the interval [1,columnnumber], array uses interval [0,columnnumber-1]
    //Cannot drop piece if it is not your turn
    if (newPiece.getOwner() != getCurrentTurn()) {
        return false;
    }
    //Cannot drop piece if the column is full
    if (m_board[0][column].isValidPiece()) {
        return false;
    }
    //Search the column (top to bottom) for where to place the piece
    for (int i=1;i<m_rowNum;i++) {
        if (m_board[i][column].isValidPiece()) {
            m_board[i-1][column] = newPiece;
            break;
        }
        if (i==m_rowNum-1) {
            m_board[i][column] = newPiece;
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
        else if (!m_board[row+moveCounter][column]) {
            break;
        }
        else if (m_board[row+moveCounter][column].getOwner() != m_board[row][column].getOwner()) {
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
        else if (!m_board[row-moveCounter][column].isValidPiece()) {
            break;
        }
        else if (m_board[row-moveCounter][column].getOwner() != m_board[row][column].getOwner()) {
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
        else if (!m_board[row][column+moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row][column+moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        else if (!m_board[row][column-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row][column-moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        if (column+moveCounter >= m_columnNum || row+moveCounter >= m_rowNum) {
            break;
        }
        else if (!m_board[row+moveCounter][column+moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row+moveCounter][column+moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        else if (!m_board[row-moveCounter][column-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row-moveCounter][column-moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        if (column+moveCounter >= m_columnNum || row-moveCounter < 0) {
            break;
        }
        else if (!m_board[row-moveCounter][column+moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row-moveCounter][column+moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        else if (!m_board[row+moveCounter][column-moveCounter].isValidPiece()) {
            break;
        }
        else if (m_board[row+moveCounter][column-moveCounter].getOwner() != m_board[row][column].getOwner()) {
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
        if (!m_board[0][column].isValidPiece()) {
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

    for (int row=0;row<m_rowNum;row++) {
        for (int column=0;column<m_columnNum;column++) {
            if (m_board[row][column].isValidPiece())
            {

                if (checkVerticalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[row][column].getOwner();
                    return;
                }

                if (checkHorizontalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[row][column].getOwner();
                    return;
                }

                if (checkUpDiagonalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[row][column].getOwner();
                    return;
                }

                if (checkDownDiagonalWin(column, row)){
                    m_gameOver =true;
                    m_winningPlayer = &m_board[row][column].getOwner();
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
    for (int row=0;row<m_rowNum;row++) {
        for (int column=0;column<m_columnNum;column++) {
            if ((getBoard())[row][column]) {
                std::cout <<" "<< (getBoard())[row][column].getOwner().getSymbol()<<" ";
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