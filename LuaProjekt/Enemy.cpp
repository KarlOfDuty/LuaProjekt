#include "Enemy.h"
#include "Player.h"
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
	this->amountOfCorners = amountOfCorners;
	this->attack = false;
	this->checkPoint = pos;
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

void Enemy::AILogic(float dt, Player *player)
{
	if (amountOfCorners == 3)
	{
		if (fabs(player->getShape().getPosition().y - shape.getPosition().y) > 0.5f)
		{
			if (player->getShape().getPosition().y > shape.getPosition().y)
			{
				shape.setPosition(shape.getPosition().x, shape.getPosition().y + 150 * dt);
			}
			else if (player->getShape().getPosition().y < shape.getPosition().y)
			{
				shape.setPosition(shape.getPosition().x, shape.getPosition().y - 150 * dt);
			}
		}

		if (fabs(player->getShape().getPosition().x - shape.getPosition().x) > 0.5f)
		{
			if (player->getShape().getPosition().x > shape.getPosition().x)
			{
				shape.setPosition(shape.getPosition().x + 150 * dt, shape.getPosition().y);
			}
			else if (player->getShape().getPosition().x < shape.getPosition().x)
			{
				shape.setPosition(shape.getPosition().x - 150 * dt, shape.getPosition().y);
			}
		}
	}
	if (amountOfCorners == 6)
	{
		std::mt19937 rng(rd());
		std::uniform_int_distribution<> point(1, 4);
		if (attack)
		{
			
		}
		if (!attack)
		{
			if (fabs(checkPoint.y - shape.getPosition().y) < 6.0f && fabs(checkPoint.x - shape.getPosition().x) < 6.0f)
			{
				std::cout << "checkpoint not reached" << std::endl;
				goTo = point(rng);

				if (goTo == 1)
				{
					checkPoint = sf::Vector2f(123, 125);
				}
				if (goTo == 2)
				{
					checkPoint = sf::Vector2f(1155, 125);
				}
				if (goTo == 3)
				{
					checkPoint = sf::Vector2f(1155, 834);
				}
				if (goTo == 4)
				{
					checkPoint = sf::Vector2f(123, 834);
				}
			}
			if (fabs(checkPoint.y - shape.getPosition().y) > 6.0f)
			{
				if (checkPoint.y > shape.getPosition().y)
				{
					shape.setPosition(shape.getPosition().x, shape.getPosition().y + 150 * dt);
				}
				else if (checkPoint.y < shape.getPosition().y)
				{
					shape.setPosition(shape.getPosition().x, shape.getPosition().y - 150 * dt);
				}
			}

			if (fabs(checkPoint.x - shape.getPosition().x) > 6.0f)
			{
				if (checkPoint.x > shape.getPosition().x)
				{
					shape.setPosition(shape.getPosition().x + 150 * dt, shape.getPosition().y);
				}
				else if (checkPoint.x < shape.getPosition().x)
				{
					shape.setPosition(shape.getPosition().x - 150 * dt, shape.getPosition().y);
				}
			}

			if (fabs(checkPoint.y - shape.getPosition().y) > 6.0f && fabs(checkPoint.x - shape.getPosition().x) > 6.0f)
			{
				attack = true;
			}
		}
	}
}

void Enemy::update(float dt, std::vector<StaticObject*> &allStaticObjects, Player *player)
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

		//AI logic: Enemy types
		AILogic(dt, player);
	}
}
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (alive)
	{
		target.draw(shape);
	}
}