#ifndef PROJECTILE_H
#define PROJECYILE_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "StaticObject.h"
#include "Player.h"

class Projectile
{
private:
	sf::RectangleShape projectile;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	int attackDamage;

public:
	Projectile();
	~Projectile();
	void update(float dt, std::vector<StaticObject*> &allStaticObjects, Player *player);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};
#endif

