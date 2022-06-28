#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{

private:
    std::string m_name;
    static int idGenerator;
protected:
    int m_id;   //Might exceed int size
public:
    Player(std::string playerName="UnnamedPlayer");

    Player(const Player& orig);

    std::string getName() const;
    int getId() const;

    bool operator==(const Player& comparedPlayer) const;
    bool operator!=(const Player& comparedPlayer) const;
};

#endif