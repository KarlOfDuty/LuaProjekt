#include "Enemy.h"

Enemy::Enemy()
{
	amountOfCorners = 0;
}

Enemy::Enemy(sf::CircleShape shape, int amountOfCorners)
{
	this->amountOfCorners = amountOfCorners;
	this->shape = shape;
}

Enemy::~Enemy()
{
	
}

int Enemy::getAmountOfCorners()
{
	return this->amountOfCorners;
}

sf::CircleShape Enemy::getShape()
{
	return this->shape;
}

void Enemy::addCorner()
{
	if (amountOfCorners < 10)
	{
		amountOfCorners++;
		sf::CircleShape newShape(10 * amountOfCorners, amountOfCorners);
		newShape.setOrigin(10 * amountOfCorners, 10 * amountOfCorners);
		newShape.rotate(45);
		newShape.setPosition(shape.getPosition());
		newShape.setFillColor(sf::Color::Red);
		shape = newShape;
	}
}
void Enemy::subtractCorner()
{
	if (amountOfCorners > 3)
	{
		amountOfCorners--;
		sf::CircleShape newShape(10 * amountOfCorners, amountOfCorners);
		newShape.setOrigin(10 * amountOfCorners, 10 * amountOfCorners);
		newShape.rotate(45);
		newShape.setPosition(shape.getPosition());
		newShape.setFillColor(sf::Color::Red);
		shape = newShape;
	}
}