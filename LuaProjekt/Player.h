#ifndef PLAYER_H
#define PLAYER_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "StaticObject.h"
#include "Projectile.h"
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
	sf::Clock shootDelay;
	std::vector<sf::RectangleShape> debugPoints;
	std::vector<Projectile> allProjectiles;
public:
	Player();
	~Player();
	sf::CircleShape getShape();
	void shoot();
	void setPos(sf::Vector2f newPos);
	void update(float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif

