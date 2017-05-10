#include "StaticObject.h"

StaticObject::StaticObject(sf::Vector2f size, sf::Vector2f pos)
{
	this->shape = sf::RectangleShape(size);
	this->shape.setPosition(pos);
}

StaticObject::~StaticObject()
{

}

sf::RectangleShape StaticObject::getShape()
{
	return shape;
}

void StaticObject::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(shape);
}