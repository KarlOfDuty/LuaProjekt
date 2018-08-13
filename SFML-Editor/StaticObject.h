#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "Tile.h"

class StaticObject : public Tile
{
private:
	int tileNumber;
public:
	StaticObject(sf::RectangleShape shape);
	StaticObject(sf::RectangleShape shape, int number);
	~StaticObject();
	int getTileNumber();
	void addNumber(std::vector<sf::Texture*> allTextures);
	void subtractNumber(std::vector<sf::Texture*> allTextures);
	std::string getTileOutput();
};

#endif // !STATICOBJECT_H
