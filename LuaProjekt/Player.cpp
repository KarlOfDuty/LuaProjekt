#include "Player.h"
#include "Enemy.h"

Player::Player()
{
	playerShape = sf::CircleShape(50, 4);
	playerShape.rotate(45);
	playerShape.setFillColor(sf::Color::Green);
	playerShape.setOrigin(playerShape.getRadius(), playerShape.getRadius());
	playerShape.setPosition(500, 200);

	attackBox = sf::RectangleShape(sf::Vector2f(20,100));
	attackBox.rotate(-45);
	attackBox.setFillColor(sf::Color(50,50,200));
	attackBox.setOrigin(10, 0);
	attacking = false;
	direction = sf::Vector2i(0, 1);

	hp = 10;

	stoppedAttacking = true;

	healthFont.loadFromFile("tiles/arial.ttf");
	healthText.setFont(healthFont);
	healthText.setString(std::to_string(hp));
}

Player::~Player()
{

}
void Player::move(float x, float y)
{
	sf::Vector2f dir;
	dir.x = x;
	dir.y = y;

	playerShape.setPosition(playerShape.getPosition() + dir);
}

int Player::movementWrapper(lua_State * L)
{
	Player* c = static_cast<Player*>(
		lua_touserdata(L, lua_upvalueindex(1)));

	sf::Vector2f dir;
	dir.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	dir.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	c->move(dir.x, dir.y);
	return 0;
}
sf::CircleShape Player::getShape()
{
	return playerShape;
}
void Player::resetHP()
{
	hp = 10;
}
void Player::applyDamage(int damage)
{
	if (invincibilityClock.getElapsedTime().asSeconds() > 0.5f)
	{
		hp -= damage;
		if (hp < 0)
		{
			hp = 0;
		}
		invincibilityClock.restart();
	}
}
void Player::shoot(lua_State* L, float dt)
{
	//Ranged attack
	lua_getglobal(L, "shoot");

	//Shot direction
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, direction.x);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, direction.y);
	lua_settable(L, -3);

	//Shot delay
	lua_pushnumber(L, shootDelay.getElapsedTime().asSeconds());

	lua_call(L, 2, 5);

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

		projectileDamage = damage;
		allProjectiles.push_back(Projectile(playerShape.getPosition(), velocity*dt, size));
		shootDelay.restart();
	}

}
void Player::setPos(sf::Vector2f newPos)
{
	this->playerShape.setPosition(newPos);
}

int Player::getInput(lua_State * L)
{
	sf::Vector2f directionVector(0,0);
	bool meleeAttack = false;
	bool rangedAttack = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		directionVector += sf::Vector2f(-1,0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		directionVector += sf::Vector2f(1,0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		directionVector += sf::Vector2f(0,-1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		directionVector += sf::Vector2f(0,1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		meleeAttack = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		rangedAttack = true;
	}
	lua_pushnumber(L, directionVector.x);
	lua_pushnumber(L, directionVector.y);
	lua_pushboolean(L, meleeAttack);
	lua_pushboolean(L, rangedAttack);
	return 4;
}

void Player::update(lua_State* L, float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects)
{
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::getInput, 1);
	lua_setglobal(L, "getInput");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::movementWrapper, 1);
	lua_setglobal(L, "playerMove");
	lua_getglobal(L, "update");
	lua_pushboolean(L, attacking);
	lua_pushnumber(L, dt);
	lua_call(L, 2, 0);

	healthText.setString(std::to_string(hp));
	//Prevent moving and using other attacks while attacking
	if (!attacking)
	{
		if (stoppedAttacking)
		{
			for (int i = 0; i < allEnemies.size(); i++)
			{
				allEnemies[i]->setMeleeCooldown(false);
			}
			stoppedAttacking = false;
		}

		sf::Vector2i movementVector;
		//Controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			direction = sf::Vector2i(-1, 0);
			movementVector = movementVector + direction;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			direction = sf::Vector2i(1, 0);
			movementVector = movementVector + direction;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			direction = sf::Vector2i(0, -1);
			movementVector = movementVector + direction;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			direction = sf::Vector2i(0, 1);
			movementVector = movementVector + direction;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			//shoot(L,dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			attacking = true;
			if (direction.x != 1)
			{
				attackBox.setRotation(270 * direction.y);
			}
			else
			{
				attackBox.setRotation(180);
			}
			attackBoxRotation = 0;
		}
		/*
		if (movementVector != sf::Vector2i(0, 0))
		{
			lua_pushlightuserdata(L, this);
			lua_pushcclosure(L, Player::movementWrapper, 1);
			lua_setglobal(L, "playerMove");
			//Movement
			lua_getglobal(L, "movement");

			//Movement vector
			lua_newtable(L);
			lua_pushstring(L, "x");
			lua_pushnumber(L, movementVector.x);
			lua_settable(L, -3);
			lua_pushstring(L, "y");
			lua_pushnumber(L, movementVector.y);
			lua_settable(L, -3);

			lua_pushnumber(L, dt);

			lua_call(L, 2, 2);
		}
		*/


	}
	else
	{
		//Rotation
		attackBox.rotate(666 * dt);
		attackBoxRotation += 666 * dt;
		if (attackBoxRotation > 180)
		{
			attacking = false;
		}

		//Collision between sword and enemy
		for (int i = 0; i < allEnemies.size(); i++)
		{
			sf::Vector2f mtv;
			if (allEnemies[i]->canTakeMeleeDamage())
			{
				if (collision::collides(attackBox, allEnemies[i]->getShape(), mtv))
				{
					allEnemies[i]->applyDamage(10);
					allEnemies[i]->setMeleeCooldown(true);
				}
			}
		}
		stoppedAttacking = true;
	}

	//Update Attackbox position
	attackBox.setPosition(playerShape.getPosition() + sf::Vector2f(35 * direction.x, 35 * direction.y));
	//Player collision with static objects
	std::vector<StaticObject*> closeObjects;
	for (int i = 0; i < allStaticObjects.size(); i++)
	{
		sf::Vector2f distanceVector = playerShape.getPosition() - allStaticObjects[i]->getCenterPos();
		float length = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
		if (length < 110)
		{
			closeObjects.push_back(allStaticObjects[i]);
		}
	}
	for (int i = 0; i < closeObjects.size(); i++)
	{
		sf::Vector2f mtv;
		if (collision::collides(closeObjects[i]->getShape(), playerShape, mtv))
		{
			playerShape.setPosition(playerShape.getPosition() - mtv);
		}
	}

	//Collision projectiles
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
			if (collision::collides(allProjectiles[i].getShape(), closeObjectsProjectiles[j]->getShape()))
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

	//Projectile collision with enemies
	for (int i = 0; i < allProjectiles.size(); i++)
	{
		for (int j = 0; j < allEnemies.size(); j++)
		{
			if (allEnemies[j]->isAlive() && collision::collides(allProjectiles[i].getShape(), allEnemies[j]->getShape()))
			{
				allEnemies[j]->applyDamage(projectileDamage);
				allProjectiles.erase(allProjectiles.begin() + i);
				j = allEnemies.size();
			}
		}
	}
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(healthText);
	if (hp > 0)
	{
		target.draw(playerShape);
		if (attacking)
		{
			target.draw(attackBox);
		}
		for (int i = 0; i < debugPoints.size(); i++)
		{
			target.draw(debugPoints[i]);
		}
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			target.draw(allProjectiles[i]);
		}
	}
}
