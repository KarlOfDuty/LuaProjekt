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
int Player::shootWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	int size = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	sf::Vector2f velocity;
	velocity.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	velocity.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	c->shoot(velocity, size);
	return 0;
}
void Player::shoot(sf::Vector2f velocity, int size)
{
	allProjectiles.push_back(Projectile(playerShape.getPosition(), velocity, size));
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
int Player::useMeleeAttackWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	sf::Vector2f direction;
	direction.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	direction.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	c->useMeleeAttack(direction);
	return 0;
}
void Player::useMeleeAttack(sf::Vector2f direction)
{
	attacking = true;
	attackBoxRotation = 0;
	attackBox.setPosition(playerShape.getPosition().x + direction.x*30, playerShape.getPosition().y+direction.y*30);
	attackBox.setRotation(atan2(-direction.y, -direction.x)/3.14*180);
}
int Player::rotateMeleeWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	float angle = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	c->rotateMelee(angle);
	return 0;
}
void Player::rotateMelee(float angle)
{
	attackBox.rotate(angle);
}
int Player::resetMeleeCooldownsWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	c->resetMeleeCooldowns();
	return 0;
}
void Player::resetMeleeCooldowns()
{
	for (int i = 0; i < allEnemies.size(); i++)
	{
		allEnemies[i]->setMeleeCooldown(false);
	}
}
int Player::meleeCollisionWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	c->meleeCollision();
	return 0;
}
void Player::meleeCollision()
{
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
}
int Player::playerCollisionWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	sf::Vector2f totalMTV = c->playerCollision();
	lua_pushnumber(L, totalMTV.x);
	lua_pushnumber(L, totalMTV.y);
	return 2;
}
sf::Vector2f Player::playerCollision()
{
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
	sf::Vector2f totalMTV(0,0);
	for (int i = 0; i < closeObjects.size(); i++)
	{
		sf::Vector2f mtv;
		if (collision::collides(closeObjects[i]->getShape(), playerShape, mtv))
		{
			totalMTV -= mtv;
		}
	}
	return totalMTV;
}
int Player::projectilesCollisionWrapper(lua_State* L)
{
	Player* c = static_cast<Player*>(lua_touserdata(L, lua_upvalueindex(1)));
	float dt = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	c->projectilesCollision(dt);
	return 0;
}
void Player::projectilesCollision(float dt)
{
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
void Player::update(lua_State* L, float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects)
{
	this->allEnemies = allEnemies;
	this->allStaticObjects = allStaticObjects;
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::getInput, 1);
	lua_setglobal(L, "getInput");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::movementWrapper, 1);
	lua_setglobal(L, "playerMove");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::useMeleeAttackWrapper, 1);
	lua_setglobal(L, "useMeleeAttack");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::shootWrapper, 1);
	lua_setglobal(L, "shoot");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::rotateMeleeWrapper, 1);
	lua_setglobal(L, "rotateMelee");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::resetMeleeCooldownsWrapper, 1);
	lua_setglobal(L, "resetMeleeCooldowns");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::meleeCollisionWrapper, 1);
	lua_setglobal(L, "meleeCollision");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::playerCollisionWrapper, 1);
	lua_setglobal(L, "playerCollision");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, Player::projectilesCollisionWrapper, 1);
	lua_setglobal(L, "projectilesCollision");
	lua_getglobal(L, "update");
	lua_pushnumber(L, dt);
	lua_call(L, 1, 1);
	attacking = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);

	healthText.setString(std::to_string(hp));
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
