#ifndef PLAYER_H
#define PLAYER_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Collision.h"
class Player : public sf::Drawable
{
private:
	sf::CircleShape playerShape;
	
	sf::RectangleShape attackBox;
	float attackBoxRotation;

	sf::Clock attackTimer;

	sf::Vector2f direction;

	bool attacking;
	int hp;

	std::vector<sf::RectangleShape> debugPoints;

public:
	Player();
	~Player();
	void update(float dt, std::vector<sf::CircleShape> &allEnemies);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif
