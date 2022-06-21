#ifndef POSITION_H
#define POSITION_H

#include "Piece.h"

class Position
{
private:
    Piece const * m_currentPiece;
    bool m_hasPiece;
public:
    Position();
    void setPiece(const Piece& placingPiece);
    Piece const & getPiece() const;
    bool hasPiece() const;
};

#endif