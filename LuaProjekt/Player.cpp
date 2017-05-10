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
	attackBox.setFillColor(sf::Color(50,50,200));
	attackBox.setOrigin(10, 0);
	attacking = false;
	direction = sf::Vector2f(0, 1);

	stoppedAttacking = false;
}

Player::~Player()
{

}
void Player::update(float dt, std::vector<Enemy*> &allEnemies)
{
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
	attackBox.setPosition(playerShape.getPosition() + sf::Vector2f(70 * direction.x, 70 * direction.y));
	
	//Player collision with enemies
	for (int i = 0; i < allEnemies.size(); i++)
	{
		sf::Vector2f mtv;
		if (allEnemies[i]->isAlive())
		{
			if (collision::collides(playerShape, allEnemies[i]->getShape(), mtv))
			{
				playerShape.setPosition(playerShape.getPosition() + mtv);
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
	for (int i = 0; i < debugPoints.size(); i++)
	{
		target.draw(debugPoints[i]);
	}
}