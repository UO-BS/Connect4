#ifndef BOT_USER_H
#define BOT_USER_H

#include "Player.h"
#include "Piece.h"

#include <array>

class BotUser : public Player
{
private:
    std::array<int,64> m_inputOwnerLayer; //-1 is Opponent piece, 0 is unoccupied, 1 is Bot's piece
    std::array<float,64> m_firstLayerWeight;
    std::array<float,8> m_outputLayer;

    void mapBoardToInputLayer(Piece**& board);
    int chooseRow();

public:
    BotUser();
    BotUser(std::array<float,64> firstLayerWeight);
    ~BotUser();
    void train();

};

#endif