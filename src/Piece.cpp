#include "Piece.h"
#include "Player.h"

#include <iostream>

const Player Piece::m_nullPlayer{};

Piece::Piece()                      : m_owner{Piece::m_nullPlayer}     , m_isNullPiece{true}{}
Piece::Piece(const Player& owner)   : m_owner{owner}            , m_isNullPiece{false}{}
Piece::Piece(const Piece& original) : m_owner{original.m_owner} , m_isNullPiece{false}{}

const Player& Piece::getOwner() const 
{
    return m_owner;
}
