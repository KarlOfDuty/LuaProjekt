#include "Projectile.h"

Projectile::Projectile()
{
	projectile.setSize(sf::Vector2f(10, 10));
	projectile.setPosition(0, 0);
	projectile.setFillColor(sf::Color::Green);
	direction = sf::Vector2f(0, 1);
}

Projectile::~Projectile()
{

}

void Projectile::update(float dt, std::vector<StaticObject*>& allStaticObjects, Player * player)
{
}

void Projectile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
}
