#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>

class Enemy : public sf::Drawable
{
private:
	int amountOfCorners;
	sf::CircleShape shape;
public:
	Enemy();
	Enemy(sf::CircleShape shape, int amountOfCorners);
	~Enemy();
	int getAmountOfCorners();
	sf::CircleShape getShape();
	void addCorner();
	void subtractCorner();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(shape);
	}
};
#endif