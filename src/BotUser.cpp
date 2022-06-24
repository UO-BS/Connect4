#include "BotUser.h"

#include <iostream>
#include <array>

BotUser::BotUser() : m_inputOwnerLayer{},m_firstLayerWeight{},m_outputLayer{}
{

}

BotUser::BotUser(std::array<float,64> firstLayerWeight) : m_inputOwnerLayer{},m_firstLayerWeight{m_firstLayerWeight},m_outputLayer{}
{

}

void BotUser::train()
{

}

int BotUser::chooseRow()
{

}

void BotUser::mapBoardToInputLayer(Piece**& board)
{
    for (int column=0;column<8;column++) {
        for (int row=0;row<8;row++) {
            if (!board[column][row].isValidPiece()) {
                m_inputOwnerLayer[column*8 + row] = 0;
            }
            if (board[column][row].getOwner() != *this) {
                m_inputOwnerLayer[column*8 + row] = -1;
            }
            if (board[column][row].getOwner() == *this) {
                m_inputOwnerLayer[column*8 + row] = 1;
            }
        }
    }
}

BotUser::~BotUser()
{
    //Write Weight data to a file
}