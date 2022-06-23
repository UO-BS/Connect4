#ifndef PIECE_H
#define PIECE_H

#include "Player.h"

class Piece {

private:
    //Pointer to the player that owns the piece. 
    //Players should outlive the game (and by extension the pieces) so this should never be an unintended dangling pointer
    Player const * m_owner;
    //Boolean for if the piece is a nullptr or not
    bool m_isValidPiece;

public:
    //Generates a null piece
    Piece();
    //Generates a valid player-owned piece
    Piece(const Player& owner);
    //Copies a piece
    Piece(const Piece& original);
    
    //Returns the owner of the piece
    const Player& getOwner() const;

    Piece& operator=(const Piece&);
    
    operator bool() const;
    bool isValidPiece() const;

};

#endif