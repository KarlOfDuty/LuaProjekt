#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include "StaticObject.h"
#include "Projectile.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
class Enemy;

class Player : public sf::Drawable
{
private:
	sf::CircleShape playerShape;
	sf::RectangleShape attackBox;
	float attackBoxRotation;
	sf::Vector2i direction;
	bool attacking;
	int hp;
	bool stoppedAttacking;
	int projectileDamage;
	sf::Clock shootDelay;
	std::vector<sf::RectangleShape> debugPoints;
	std::vector<Projectile> allProjectiles;
	sf::Clock invincibilityClock;

	sf::Font healthFont;
	sf::Text healthText;
	static int movementWrapper(lua_State* L);
	static int getInput(lua_State* L);
public:
	Player();
	~Player();
	sf::CircleShape getShape();
	void resetHP();
	void applyDamage(int damage);
	void shoot(lua_State* L, float dt);
	void setPos(sf::Vector2f newPos);
	void move(float x, float y);
	void update(lua_State* L, float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};

#endif
