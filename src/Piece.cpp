#include "Piece.h"
#include "Player.h"

Piece::Piece()                      : m_owner{nullptr}          , m_isValidPiece{false}{}
Piece::Piece(const Player& owner)   : m_owner{&owner}           , m_isValidPiece{true}{}
Piece::Piece(const Piece& original) : m_owner{original.m_owner} , m_isValidPiece{original.m_isValidPiece}{}

const Player& Piece::getOwner() const 
{
    return *m_owner;
}

Piece& Piece::operator=(const Piece& other) 
{
    //self-assignment?
    m_owner = other.m_owner;
    m_isValidPiece = other.m_isValidPiece;
    return *this;
}

bool Piece::isValidPiece() const
{
    return m_isValidPiece;
}

Piece::operator bool() const
{
    return m_isValidPiece;
}