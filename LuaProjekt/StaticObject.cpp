#include "StaticObject.h"

StaticObject::StaticObject(sf::Vector2f pos)
{
	this->shape = sf::RectangleShape(sf::Vector2f(80,80));
	this->shape.setPosition(pos);
}

StaticObject::~StaticObject()
{

}

sf::RectangleShape StaticObject::getShape()
{
	return shape;
}

sf::Vector2f StaticObject::getCenterPos()
{
	return shape.getPosition() + sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2);
}

void StaticObject::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(shape);
}