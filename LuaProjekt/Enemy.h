#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include "StaticObject.h"
#include "Collision.h"
#include <random>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
class Player;

class Enemy : public :: sf::Drawable
{
private:
	sf::CircleShape shape;
	int health;
	int damage;
	bool alive;
	bool meleeDamage;
	int amountOfCorners;
	bool attack;
	int goTo;
	sf::Vector2f checkPoint;
	std::random_device rd;
public:
	Enemy(int radius, int amountOfCorners, int health, int damage, sf::Color color, sf::Vector2f pos);
	~Enemy();
	sf::CircleShape getShape();
	bool isAlive();
	bool canTakeMeleeDamage();
	void setMeleeCooldown(bool canTakeDmg);
	void applyDamage(int damageTaken);
	void update(lua_State* L, float dt, std::vector<StaticObject*> &allStaticObjects, Player *player);
	void move(sf::Vector2f dir);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif

