#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int radius, int amountOfCorners, int health, int damage, sf::Color color, sf::Vector2f pos)
{
	this->shape = sf::CircleShape(radius, amountOfCorners);
	this->shape.setOrigin(radius, radius);
	this->shape.rotate(45);
	this->shape.setPosition(pos);
	this->shape.setFillColor(color);
	this->health = health;
	this->damage = damage;
	this->alive = true;
}

Enemy::~Enemy()
{

}
sf::CircleShape Enemy::getShape()
{
	return shape;
}
bool Enemy::isAlive()
{
	return alive;
}
bool Enemy::canTakeMeleeDamage()
{
	return alive && meleeDamage;
}
void Enemy::setMeleeCooldown(bool canTakeDmg)
{
	this->meleeDamage = !canTakeDmg;
}
void Enemy::applyDamage(int damageTaken)
{
	this->health -= damageTaken;
	if (health <= 0)
	{
		alive = false;
	}
}
void Enemy::update(float dt)
{
	if (alive)
	{

	}
}
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (alive)
	{
		target.draw(shape);
	}
}