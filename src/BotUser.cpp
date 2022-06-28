#include "BotUser.h"

#include <array>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

float BotUser::sigmoid(float input)
{
    //std::cout << (1.0 / (1.0 + exp(-input)));
    return (1.0 / (1.0 + exp(-input)));
}

BotUser::BotUser() : Player(), m_inputOwnerLayer{0},m_firstLayerWeight{0},m_firstLayer{0},m_secondLayerWeight{0},m_secondLayer{0},m_outputLayerWeight{0},m_outputLayer{0}
{

}

BotUser::BotUser(const std::string& fileName) : BotUser()
{
    std::ifstream inf(fileName, std::ios::in); //CHECK FOR VALID FILE NAME
    std::string temp;
    
    std::getline(inf, temp);
    populateArraysFromFile(temp,m_inputOwnerLayer);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_firstLayerWeight);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_firstLayer);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_secondLayerWeight);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_secondLayer);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_outputLayerWeight);

    std::getline(inf, temp);
    populateArraysFromFile(temp,m_outputLayer);
    

    inf.close();
}

void BotUser::train(bool won)
{
    //THIS IS TERRIBLE, WILL BE FIXED EXTENSIVELY
    //The last 3 moves are the most important, then 10?
    short signModifier = -1;
    if (won) {
        signModifier = 1;
    }

    for (int lastMoves=m_choiceHistory.size()-1;lastMoves>0;lastMoves--) {
        for (int secondLayer=0;secondLayer<m_secondLayer.size();secondLayer++) {
            m_outputLayerWeight[m_choiceHistory[lastMoves]*m_secondLayer.size() + secondLayer] += 0.1*signModifier; //Flat increase/decrease
        }
    }
        
}

int BotUser::chooseColumn(Piece**const& board)
{
    mapBoardToInputLayer(board);
    
    for (int firstLayer=0;firstLayer<m_firstLayer.size();firstLayer++) {
        float temp{};
        for (int inputLayer=0;inputLayer<m_inputOwnerLayer.size();inputLayer++) {
            temp += (m_inputOwnerLayer[inputLayer] * m_firstLayerWeight[firstLayer*m_inputOwnerLayer.size() + inputLayer]) ;
        }
        m_firstLayer[firstLayer] = sigmoid(temp);
    }

    for (int secondLayer=0;secondLayer<m_secondLayer.size();secondLayer++) {
        float temp{};
        for (int firstLayer=0;firstLayer<m_firstLayer.size();firstLayer++) {
            temp += m_firstLayer[firstLayer] * m_secondLayerWeight[secondLayer*m_firstLayer.size() + firstLayer] ;
        }
        m_secondLayer[secondLayer] = sigmoid(temp);
    }

    for (int outputLayer=0;outputLayer<m_outputLayer.size();outputLayer++) {
        float temp{};
        for (int secondLayer=0;secondLayer<m_secondLayer.size();secondLayer++) {
            temp += m_secondLayer[secondLayer] * m_outputLayerWeight[outputLayer*m_secondLayer.size() + secondLayer] ;
        }
        m_outputLayer[outputLayer] = sigmoid(temp);
    }

    int chosenColumn{-1};
    int chosenRow{-1};
    for (int column=0;column<8;column++) {
        for (int row=7;row>=0;row--) {
            if (board[column][row].isValidPiece() && row!=7) {
                if (m_outputLayer[column*8 + row+1] > m_outputLayer[chosenColumn*8 + chosenRow]){
                    chosenColumn = column;
                    chosenRow = row+1;
                }
            } else
            if (m_outputLayer[column*8 + row+1] > m_outputLayer[chosenColumn*8 + chosenRow] && row==0) {
                chosenColumn = column;
                chosenRow = row;
            }
        }
    }

    m_choiceHistory.push_back(chosenColumn*8 + chosenRow);
    return chosenColumn;

}

void BotUser::mapBoardToInputLayer(Piece**const& board)
{
    for (int column=0;column<8;column++) {
        for (int row=0;row<8;row++) {
            if (!board[column][row].isValidPiece()) {
                m_inputOwnerLayer[column*8 + row] = 0;
            } else
            if (board[column][row].getOwner() != *this) {
                m_inputOwnerLayer[column*8 + row] = -1;
            } else
            if (board[column][row].getOwner() == *this) {
                m_inputOwnerLayer[column*8 + row] = 1;
            }
            
        }
    }
}

BotUser::~BotUser()
{
    //Write Weight data to a file
    std::ofstream outf{"weight.txt", std::ios::out};

    for (auto value : m_inputOwnerLayer) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_firstLayerWeight) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_firstLayer) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_secondLayerWeight) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_secondLayer) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_outputLayerWeight) {
        outf << value << " ";
    } 
    outf << "\n";
    for (auto value : m_outputLayer) {
        outf << value << " ";
    } 
    outf << "\n";

    outf.close();
    
}
