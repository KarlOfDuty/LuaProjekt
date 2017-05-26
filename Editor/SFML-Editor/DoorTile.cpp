#include "DoorTile.h"

DoorTile::DoorTile(sf::RectangleShape tileShape) : Tile(tileShape)
{
	mapNum = 1;
	mapName = "map" + std::to_string(mapNum);
	allOutDirections.push_back("W");
	allOutDirections.push_back("N");
	allOutDirections.push_back("E");
	allOutDirections.push_back("S");
	outDirection = allOutDirections[0];

	allTileNames.push_back("W");
	allTileNames.push_back("N");
	allTileNames.push_back("E");
	allTileNames.push_back("S");
	tileName = allTileNames[0];
}
void DoorTile::setTileName(std::string tileName)
{
	this->tileName = tileName;
}
void DoorTile::increaseMapNum(int mapNum)
{
	this->mapNum = (this->mapNum + mapNum);
	setMapName();
}
void DoorTile::decreaseMapNum(int mapNum)
{
	this->mapNum = (this->mapNum - mapNum);
	setMapName();
}
void DoorTile::setMapName()
{
	this->mapName = "map";
	this->mapName = this->mapName + std::to_string(mapNum);
}
void DoorTile::increaseMapDir()
{
	if (getCurrentDir() < 3)
	{
		this->outDirection = allOutDirections[getCurrentDir() + 1];
	}
}
void DoorTile::decreaseMapDir()
{
	if (getCurrentDir() > 0)
	{
		this->outDirection = allOutDirections[getCurrentDir() - 1];
	}
}
int DoorTile::getCurrentDir()
{
	if (outDirection == allOutDirections[0])
	{
		return 0;
	}
	if (outDirection == allOutDirections[1])
	{
		return 1;
	}
	if (outDirection == allOutDirections[2])
	{
		return 2;
	}
	if (outDirection == allOutDirections[3])
	{
		return 3;
	}
}
void DoorTile::increaseTileName()
{
	if (getCurrentTileName() < 3)
	{
		this->tileName = allTileNames[getCurrentTileName() + 1];
	}
}
void DoorTile::decreaseTileName()
{
	if (getCurrentTileName() > 0)
	{
		this->tileName = allTileNames[getCurrentTileName() - 1];
	}
}
int DoorTile::getCurrentTileName()
{
	if (tileName == allTileNames[0])
	{
		return 0;
	}
	if (tileName == allTileNames[1])
	{
		return 1;
	}
	if (tileName == allTileNames[2])
	{
		return 2;
	}
	if (tileName == allTileNames[3])
	{
		return 3;
	}
}
std::string DoorTile::getTileName()
{
	return tileName;
}
int DoorTile::getMapNum()
{
	return mapNum;
}
std::string DoorTile::getMapName()
{
	return mapName;
}
std::string DoorTile::getOutDirection()
{
	return outDirection;
}
