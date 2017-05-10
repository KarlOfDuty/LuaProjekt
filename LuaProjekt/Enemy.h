#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>

class Enemy : public :: sf::Drawable
{
private:
	sf::CircleShape shape;
	int health;
	int damage;
	bool alive;
	bool meleeDamage;
public:
	Enemy(int radius, int amountOfCorners, int health, int damage, sf::Vector2f pos);
	~Enemy();
	sf::CircleShape getShape();
	bool isAlive();
	bool canTakeMeleeDamage();
	void setMeleeCooldown(bool canTakeDmg);
	void applyDamage(int damageTaken);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif

