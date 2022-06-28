#ifndef BOT_USER_H
#define BOT_USER_H

#include "Player.h"
#include "Piece.h"

#include <array>
#include <vector>
#include <utility>
#include <string>
#include <iostream>

class BotUser : public Player
{
private:
    // Input layer (64) -> 4x4 layer (5X5)  -> 2x2 layer (7X7) -> individual layer (8X8) -> choose from the 8 rows
    std::array<float,64>    m_inputOwnerLayer; //-1 is Opponent piece, 0 is unoccupied, 1 is Bot's piece
    std::array<float,1600>  m_firstLayerWeight;
    std::array<float,25>    m_firstLayer;
    std::array<float,1225>  m_secondLayerWeight;
    std::array<float,49>    m_secondLayer;
    std::array<float,3136>  m_outputLayerWeight;
    std::array<float,64>    m_outputLayer;

    std::vector<int> m_choiceHistory;

    void mapBoardToInputLayer(Piece**const& board);
    
    float sigmoid(float input);

    template<std::size_t SIZE>
    void populateArraysFromFile(const std::string& line, std::array<float, SIZE>& array)
    {
        size_t lastPosition = 0;
        size_t currentPosition = 0;
        std::string delimiter = " ";
        int counter=0;

        while ((currentPosition = line.find(delimiter, lastPosition)) != std::string::npos) {
            array[counter] = std::stof( line.substr(lastPosition, currentPosition-lastPosition));
            lastPosition = currentPosition +1;
            counter++;
        }
    }

public:
    BotUser();
    BotUser(const std::string& fileName);
    ~BotUser();

    void train(bool won);

    int chooseColumn(Piece**const& board);
};

#endif