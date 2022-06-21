#ifndef PIECE_H
#define PIECE_H

#include "Player.h"

class Piece {

private:
    //This is a blank player object to be used by all null pieces
    const static Player m_nullPlayer;

    //reference to the Player who owns this piece
    const Player& m_owner;

    //Boolean to determine wether this piece is valid
    bool m_isNullPiece;

public:
    //Default Constructor: Generates a null piece
    Piece();
    //Generates a valid player-owned piece
    Piece(const Player& owner);
    //Copies a piece
    Piece(const Piece& original);
    
    //Returns the owner of the piece
    const Player& getOwner() const;

};

#endif