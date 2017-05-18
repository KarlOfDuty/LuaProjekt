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
Enemy::Enemy(int nr, sf::Vector2f pos)
{
	this->shape = sf::CircleShape(50, nr);
	this->shape.setOrigin(50, 50);
	this->shape.rotate(45);
	this->shape.setPosition(pos);
	this->shape.setFillColor(sf::Color::Red);
	this->health = 7*nr;
	this->damage = nr;
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
void Enemy::update(float dt, std::vector<StaticObject*> &allStaticObjects)
{
	if (alive)
	{	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shape.setPosition(shape.getPosition().x - 300 * dt, shape.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			shape.setPosition(shape.getPosition().x + 300 * dt, shape.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			shape.setPosition(shape.getPosition().x, shape.getPosition().y - 300 * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			shape.setPosition(shape.getPosition().x, shape.getPosition().y + 300 * dt);
		}
		//Collision with static objects
		std::vector<StaticObject*> closeObjects;
		for (int i = 0; i < allStaticObjects.size(); i++)
		{
			sf::Vector2f distanceVector = shape.getPosition() - allStaticObjects[i]->getCenterPos();
			float length = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
			if (length < 150)
			{
				closeObjects.push_back(allStaticObjects[i]);
			}
		}
		{
			for (int i = 0; i < closeObjects.size(); i++)
			{
				sf::Vector2f mtv;
				if (collision::collides(closeObjects[i]->getShape(), shape, mtv))
				{
					shape.setPosition(shape.getPosition() - mtv);
				}
			}
		}
	}
}
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (alive)
	{
		target.draw(shape);
	}
}