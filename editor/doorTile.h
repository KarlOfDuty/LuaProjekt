#ifndef DOORTILE_H
#define DOORTILE_H
#include "tile.h"
class DoorTile : public Tile
{
private:
    std::string tileName;
    std::string mapName;
    std::string outDirection;
public:
    DoorTile(sf::RectangleShape tileShape);
    void setTileName(std::string tileName);
    void setMapName(std::string mapName);
    void setOutDirection(std::string outDirection);
    std::string getTileName();
    std::string getMapName();
    std::string getOutDirection();
};

#endif
