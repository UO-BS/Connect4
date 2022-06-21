#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{

private:
    std::string m_name;
public:
    Player(std::string playerName="nullPlayer");

    Player(const Player& orig);

    std::string getName() const;
};

#endif