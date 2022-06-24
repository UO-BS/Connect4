#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{

private:
    std::string m_name;
    static int idGenerator;
    int m_id;
public:
    Player(std::string playerName="UnnamedPlayer");

    Player(const Player& orig);

    std::string getName() const;

    bool operator==(const Player& comparedPlayer) const;
    bool operator!=(const Player& comparedPlayer) const;
};

#endif