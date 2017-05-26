#ifndef DOORTILE_H
#define DOORTILE_H

#include "Tile.h"

class DoorTile : public Tile
{
private:
	std::string tileName;
	std::string mapName;
	int mapNum;
	std::string outDirection;
	std::vector<std::string> allOutDirections;
	std::vector<std::string> allTileNames;
public:
	DoorTile(sf::RectangleShape tileShape);
	void setTileName(std::string tileName);
	void increaseMapNum(int mapNum);
	void decreaseMapNum(int mapNum);
	void setMapName();
	void increaseMapDir();
	void decreaseMapDir();
	int getCurrentDir();
	void increaseTileName();
	void decreaseTileName();
	int getCurrentTileName();
	std::string getTileName();
	int getMapNum();
	std::string getMapName();
	std::string getOutDirection();
};

#endif
