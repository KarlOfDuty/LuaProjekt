#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <SFML\Graphics.hpp>

class StaticObject : public sf::Drawable
{
private:
	sf::RectangleShape shape;
public:
	StaticObject(sf::Vector2f size, sf::Vector2f pos);
	~StaticObject();
	sf::RectangleShape getShape();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif