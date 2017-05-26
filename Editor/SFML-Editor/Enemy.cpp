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
