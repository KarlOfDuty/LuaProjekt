#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable
{
public:
	sf::RectangleShape shape;
	Tile(sf::RectangleShape shape)
	{
		this->shape = shape;
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(shape);
	}
};

#endif
