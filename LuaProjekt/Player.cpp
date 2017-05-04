#include "Player.h"
Player::Player()
{
	playerShape = sf::CircleShape(100, 4);
	playerShape.rotate(45);
	playerShape.setFillColor(sf::Color::Green);
	playerShape.setOrigin(playerShape.getRadius(), playerShape.getRadius());
	playerShape.setPosition(500, 200);

	attackBox = sf::RectangleShape(sf::Vector2f(20,150));
	attackBox.rotate(-45);
	attackBox.setFillColor(sf::Color::Green);
	attackBox.setOrigin(10, 0);
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
		//Controls
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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
			attackBoxRotation = 0;
		}
	}
	else
	{
		//Rotation
		attackBox.rotate(450 * dt);
		attackBoxRotation += 450 * dt;
		if (attackBoxRotation > 180)
		{
			attacking = false;
		}

		//Collision between sword and enemy
		for (int i = 0; i < allEnemies.size(); i++)
		{
			sf::Vector2f mtv;
			if (collision::collides(attackBox, allEnemies[i], mtv))
			{
				//SWORD COLLIDES WITH ENEMY
			}
		}
	}

	//Update Attackbox position
	attackBox.setPosition(playerShape.getPosition() + sf::Vector2f(70 * direction.x, 70 * direction.y));
	
	//Player collision with enemies
	for (int i = 0; i < allEnemies.size(); i++)
	{
		sf::Vector2f mtv;
		if (collision::collides(playerShape, allEnemies[i], mtv))
		{
			playerShape.setPosition(playerShape.getPosition() + mtv);
		}
	}

	/*if (debugPoints.empty())
	{
		for (int i = 0; i < playerShape.getPointCount(); i++)
		{
			debugPoints.push_back(sf::RectangleShape(sf::Vector2f(20, 20)));
			debugPoints[i].setOrigin(10, 10);
			float rotationShape1 = playerShape.getRotation() * 3.1415926535897 / 180;
			sf::Vector2f centerShape1 = playerShape.getPosition();
			sf::Vector2f point(playerShape.getPosition() + (playerShape.getPoint(i)) - sf::Vector2f(playerShape.getRadius(), playerShape.getRadius()));
			float x = centerShape1.x + (point.x - centerShape1.x) * cos(rotationShape1) - (point.y - centerShape1.y) * sin(rotationShape1);
			float y = centerShape1.y + (point.x - centerShape1.x) * sin(rotationShape1) + (point.y - centerShape1.y) * cos(rotationShape1);
			debugPoints[i].setPosition(x, y);
		}
	}
	else
	{
		for (int i = 0; i < playerShape.getPointCount(); i++)
		{
			std::vector<sf::Vector2f> points = collision::getPoints(attackBox);
			debugPoints[i].setPosition(points[i]);
		}
	}*/
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const
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
}