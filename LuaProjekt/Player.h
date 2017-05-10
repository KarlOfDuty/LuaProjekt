#ifndef PLAYER_H
#define PLAYER_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "Enemy.h"
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
	void update(float dt, std::vector<Enemy*> &allEnemies);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif

