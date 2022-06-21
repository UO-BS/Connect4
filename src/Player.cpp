#include "Player.h"

#include <iostream>
#include <string>

std::string Player::getName() const
{
    return m_name;
}

Player::Player(const Player& original) : m_name{original.m_name} {};

Player::Player(std::string playerName) : m_name{playerName} {};