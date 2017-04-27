#include "Player.h"
Player::Player()
{
	playerShape = sf::CircleShape(100, 4);
	playerShape.rotate(45);
	playerShape.setFillColor(sf::Color::Green);
	playerShape.setOrigin(100, 100);
	playerShape.setPosition(500, 300);

	attackBox = sf::CircleShape(100,4);
	attackBox.rotate(-45);
	attackBox.setFillColor(sf::Color::Green);
	attackBox.setOrigin(15, 0);
	attacking = false;
	direction = sf::Vector2f(0, 1);
}

Player::~Player()
{

}
void Player::update(float dt, std::vector<sf::CircleShape> &allEnemies)
{
	if (!attacking)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerShape.setPosition(playerShape.getPosition().x - 300 * dt, playerShape.getPosition().y);
			direction = sf::Vector2f(-1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerShape.setPosition(playerShape.getPosition().x + 300 * dt, playerShape.getPosition().y);
			direction = sf::Vector2f(1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y - 300 * dt);
			direction = sf::Vector2f(0, -1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerShape.setPosition(playerShape.getPosition().x, playerShape.getPosition().y + 300 * dt);
			direction = sf::Vector2f(0, 1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && attacking == false)
		{
			attackTimer.restart();
			attacking = true;
			if (direction.x != 1)
			{
				attackBox.setRotation(270 * direction.y);
			}
			else
			{
				attackBox.setRotation(180);
			}
		}
		for (int i = 0; i < allEnemies.size(); i++)
		{
			sf::Vector2f mtv;
			if (collision::collides(playerShape, allEnemies[i], mtv))
			{
				playerShape.setPosition(playerShape.getPosition() + mtv);
			}
		}
		attackBox.setPosition(playerShape.getPosition() + sf::Vector2f(70 * direction.x, 70 * direction.y));
	}
	else
	{
		if (attackTimer.getElapsedTime().asSeconds() > 0.355f)
		{
			attacking = false;
		}
		attackBox.setPosition(playerShape.getPosition() + sf::Vector2f(70 * direction.x, 70 * direction.y));
		attackBox.rotate(450 * dt);
		for (int i = 0; i < allEnemies.size(); i++)
		{
			sf::Vector2f mtv;
			if (collision::collides(attackBox, allEnemies[i], mtv))
			{
				allEnemies[i].setPosition(allEnemies[i].getPosition() - mtv);
			}
		}
	}
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(playerShape);
	if (attacking)
	{
		target.draw(attackBox);
	}
}