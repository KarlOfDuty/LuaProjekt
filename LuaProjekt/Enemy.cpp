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
void Enemy::update(lua_State* L, float dt, std::vector<StaticObject*> &allStaticObjects, Player *player, std::vector<Enemy*> enemies)
{
	if (alive)
	{	
		lua_getglobal(L, "movement");

		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, shape.getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, shape.getPosition().y);
		lua_settable(L, -3);

		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, player->getShape().getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, player->getShape().getPosition().y);
		lua_settable(L, -3);

		lua_call(L, 2, 2);

		sf::Vector2f dir;
		dir.x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		dir.y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		move(dir*dt);

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
		for (int i = 0; i < closeObjects.size(); i++)
		{
			sf::Vector2f mtv;
			if (collision::collides(closeObjects[i]->getShape(), shape, mtv))
			{
				shape.setPosition(shape.getPosition() - mtv);
			}
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
void Enemy::move(sf::Vector2f dir)
{
	shape.setPosition(shape.getPosition() + dir);
}
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (alive)
	{
		target.draw(shape);
	}
}