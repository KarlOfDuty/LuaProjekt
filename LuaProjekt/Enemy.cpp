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
}
Enemy::Enemy(int nr, sf::Vector2f pos)
{
	this->shape = sf::CircleShape(10*nr, nr);
	this->shape.setOrigin(10*nr, 10*nr);
	this->shape.rotate(45);
	this->shape.setPosition(pos);
	this->shape.setFillColor(sf::Color::Red);
	this->health = (nr-2)*nr;
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
void Enemy::rangedAttack(sf::Vector2f velocity, int damage, int size, Player* player)
{
	//Create projectile here
	this->damage = damage;
	allProjectiles.push_back(Projectile(shape.getPosition(), velocity, size));
	timeSinceLastShot = 0;
}
void Enemy::update(lua_State* L, float dt, std::vector<StaticObject*> &allStaticObjects, Player *player, std::vector<Enemy*> enemies)
{
	timeSinceLastShot += dt;
	if (alive)
	{	
		//Movement
		lua_getglobal(L, "enemyMovement");

		//This pos
		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, shape.getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, shape.getPosition().y);
		lua_settable(L, -3);

		//Player pos
		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, player->getShape().getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, player->getShape().getPosition().y);
		lua_settable(L, -3);

		lua_pushnumber(L, shape.getPointCount());

		lua_call(L, 3, 2);

		sf::Vector2f dir;
		dir.x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		dir.y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		move(dir*dt);

		//Ranged attack
		lua_getglobal(L, "rangedAttackAI");

		//This pos
		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, shape.getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, shape.getPosition().y);
		lua_settable(L, -3);

		//Player pos
		lua_newtable(L);
		lua_pushstring(L, "x");
		lua_pushnumber(L, player->getShape().getPosition().x);
		lua_settable(L, -3);
		lua_pushstring(L, "y");
		lua_pushnumber(L, player->getShape().getPosition().y);
		lua_settable(L, -3);

		//Numbers
		lua_pushnumber(L, timeSinceLastShot);
		lua_pushnumber(L, shape.getPointCount());
		lua_call(L, 4, 5);

		bool shoot = true;
		shoot = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);
		if (shoot)
		{
			sf::Vector2f velocity;
			velocity.x = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			velocity.y = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);

			int damage = 0;
			damage = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);

			int size = 0;
			size = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);
			rangedAttack(velocity*dt, damage, size, player);
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


		//Collision for projectiles
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			allProjectiles[i].update(dt);
			std::vector<StaticObject*> closeObjectsProjectiles;
			for (int j = 0; j < allStaticObjects.size(); j++)
			{
				sf::Vector2f distanceVector = allProjectiles[i].getShape().getPosition() - allStaticObjects[j]->getCenterPos();
				float length = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
				if (length < 110)
				{
					closeObjectsProjectiles.push_back(allStaticObjects[j]);
				}
			}
			for (int j = 0; j < closeObjectsProjectiles.size(); j++)
			{
				if (collision::collides(allProjectiles[i].getShape(), closeObjectsProjectiles[j]->getShape(), sf::Vector2f()))
				{
					allProjectiles.erase(allProjectiles.begin() + i);
					j = closeObjectsProjectiles.size();
				}
			}
		}

		//Check if out of bounds
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			if (allProjectiles[i].getShape().getPosition().x < 0 ||
				allProjectiles[i].getShape().getPosition().x > 1280 ||
				allProjectiles[i].getShape().getPosition().y < 0 ||
				allProjectiles[i].getShape().getPosition().y > 960)
			{
				allProjectiles.erase(allProjectiles.begin() + i);
			}
		}

		for (int i = 0; i < allProjectiles.size(); i++)
		{
			if (collision::collides(allProjectiles[i].getShape(), player->getShape(), sf::Vector2f()))
			{
				player->applyDamage(damage);
				allProjectiles.erase(allProjectiles.begin() + i);
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
		
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			target.draw(allProjectiles[i]);
		}
	}
}