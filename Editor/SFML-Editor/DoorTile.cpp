#include "DoorTile.h"

DoorTile::DoorTile(sf::RectangleShape tileShape) : Tile(tileShape)
{
	tileName = "tile";
	mapName = "map";
	outDirection = "N";
}
void DoorTile::setTileName(std::string tileName)
{
	this->tileName = tileName;
}
void DoorTile::setMapName(std::string mapName)
{
	this->mapName = mapName;
}
void DoorTile::setOutDirection(std::string outDirection)
{
	this->outDirection = outDirection;
}
std::string DoorTile::getTileName()
{
	return tileName;
}
std::string DoorTile::getMapName()
{
	return mapName;
}
std::string DoorTile::getOutDirection()
{
	return outDirection;
}
