#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "StaticObject.h"
#include "Collision.h"
#include <random>
#include "Projectile.h"
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
	float timeSinceLastShot;

	std::vector<StaticObject*> allStaticObjects;
	std::vector<Projectile> allProjectiles;
	Player* player;

	static int functionWrapper(lua_State* L);

public:
	Enemy(int radius, int amountOfCorners, int health, int damage, sf::Color color, sf::Vector2f pos);
	Enemy(int nr, sf::Vector2f pos);
	~Enemy();
	sf::CircleShape getShape();
	bool isAlive();
	bool canTakeMeleeDamage();
	void setMeleeCooldown(bool canTakeDmg);
	void applyDamage(int damageTaken);
	void rangedAttack(sf::Vector2f velocity, int damage, int size, Player* player);
	void update(lua_State* L, float dt, std::vector<StaticObject*> &allStaticObjects, Player *player, std::vector<Enemy*> enemies);
	int worldCollision();
	int projectilesCollision(int index);
	void move(float x, float y);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif
