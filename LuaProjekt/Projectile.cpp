#include "Projectile.h"

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f velocity, int size)
{
	projectile.setSize(sf::Vector2f(size, size));
	projectile.setPosition(pos);
	projectile.setFillColor(sf::Color::Green);
	this->velocity = velocity;
}

Projectile::~Projectile()
{

}

sf::RectangleShape Projectile::getShape()
{
	return projectile;
}

void Projectile::update(float dt)
{
	projectile.setPosition(projectile.getPosition().x + velocity.x*dt, projectile.getPosition().y + velocity.y*dt);
}

void Projectile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(projectile);
}
