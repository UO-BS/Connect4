#include "Player.h"
#include <string>

int Player::idGenerator=1;

std::string Player::getName() const
{
    return m_name;
}

Player::Player(const Player& original) : m_id{original.m_id}, m_name{original.m_name} {}

Player::Player(std::string playerName) : m_id{idGenerator++}, m_name{playerName} {}

bool Player::operator==(const Player& comparedPlayer) const
{
    return (m_id==comparedPlayer.m_id);
}
bool Player::operator!=(const Player& comparedPlayer) const {return !(*this == comparedPlayer); }