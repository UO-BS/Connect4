#include "Position.h"

Position::Position() : m_hasPiece{false}
{
    m_currentPiece = nullptr;
}

void Position::setPiece(const Piece& placingPiece)
{
    m_currentPiece = &placingPiece;
    m_hasPiece =true;
}

Piece const & Position::getPiece() const
{
    return *m_currentPiece;
}

bool Position::hasPiece() const
{
    return m_hasPiece;
}