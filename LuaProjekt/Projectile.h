#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "StaticObject.h"

class Projectile : public sf::Drawable
{
private:
	sf::RectangleShape projectile;
	sf::Vector2f velocity;
	int attackDamage;
public:
	Projectile(sf::Vector2f pos, sf::Vector2f velocity, int size);
	~Projectile();
	sf::RectangleShape getShape();
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};
#endif