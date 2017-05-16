#ifndef PLAYER_H
#define PLAYER_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "StaticObject.h"

class Enemy;

class Player : public sf::Drawable
{
private:
	sf::CircleShape playerShape;
	sf::RectangleShape attackBox;
	float attackBoxRotation;
	sf::Vector2f direction;
	bool attacking;
	int hp;
	bool stoppedAttacking;
	std::vector<sf::RectangleShape> debugPoints;

public:
	Player();
	~Player();
	sf::CircleShape getShape();
	void setPos(sf::Vector2f newPos);
	void update(float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif

