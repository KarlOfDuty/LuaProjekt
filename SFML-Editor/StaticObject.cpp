#include "StaticObject.h"

StaticObject::StaticObject(sf::RectangleShape shape) : Tile(shape)
{
	tileNumber = 0;
}

StaticObject::StaticObject(sf::RectangleShape shape, int number) : Tile(shape)
{
	tileNumber = number;
}

StaticObject::~StaticObject()
{

}

int StaticObject::getTileNumber()
{
	return tileNumber;
}

void StaticObject::addNumber(std::vector<sf::Texture*> allTextures)
{
	if (tileNumber < 7)
	{
		tileNumber++;
		shape.setTexture(allTextures[tileNumber]);
	}
}

void StaticObject::subtractNumber(std::vector<sf::Texture*> allTextures)
{
	if (tileNumber > 0)
	{
		tileNumber--;
		shape.setTexture(allTextures[tileNumber]);
	}
}

std::string StaticObject::getTileOutput()
{
	return std::to_string(tileNumber);
}
