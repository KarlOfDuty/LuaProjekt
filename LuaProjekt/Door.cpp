#include "Door.h"

Door::Door(sf::Vector2f pos, sf::Vector2f playerNewPos, std::string mapName)
{
	this->shape = sf::RectangleShape(sf::Vector2f(80,80));
	this->shape.setFillColor(sf::Color::Magenta);
	this->shape.setPosition(pos);
	active = false;
	this->playerNewPos = playerNewPos;
	this->mapName = mapName;
}


Door::~Door()
{
}

sf::RectangleShape& Door::getShape()
{
	return shape;
}
sf::Vector2f Door::getCenterPos()
{
	return shape.getPosition() + sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2);
}

sf::Vector2f Door::getPlayerNewPos()
{
	return playerNewPos;
}

std::string Door::getMapName()
{
	return mapName;
}

bool Door::isActive()
{
	return active;
}

void Door::setActive(bool active)
{
	this->active = active;
}

void Door::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(shape);
}
