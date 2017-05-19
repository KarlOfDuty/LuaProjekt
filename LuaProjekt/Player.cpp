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
	direction = sf::Vector2f(0, 1);

	stoppedAttacking = false;
}

Player::~Player()
{

}
sf::CircleShape Player::getShape()
{
	return playerShape;
}
void Player::shoot()
{
	if (shootDelay.getElapsedTime().asSeconds() >= 0.6)
	{
		allProjectiles.push_back(Projectile(playerShape.getPosition(), direction*900.0f, 15));
		shootDelay.restart();
	}
}
void Player::setPos(sf::Vector2f newPos)
{
	this->playerShape.setPosition(newPos);
}
void Player::update(float dt, std::vector<Enemy*> &allEnemies, std::vector<StaticObject*> &allStaticObjects)
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			shoot();
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
	
	//Player collision with enemies
	for (int i = 0; i < allEnemies.size(); i++)
	{
		sf::Vector2f mtv;
		if (allEnemies[i]->isAlive())
		{
			if (collision::collides(playerShape, allEnemies[i]->getShape(), mtv))
			{
				//playerShape.setPosition(playerShape.getPosition() + mtv);
			}
		}
	}
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
			if (collision::collides(allProjectiles[i].getShape(), closeObjectsProjectiles[j]->getShape(), sf::Vector2f()))
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

	//Collision with enemies
	for (int i = 0; i < allProjectiles.size(); i++)
	{
		for (int j = 0; j < allEnemies.size(); j++)
		{
			if (collision::collides(allProjectiles[i].getShape(), allEnemies[j]->getShape(), sf::Vector2f()))
			{
				allEnemies[j]->applyDamage(5);
				allProjectiles.erase(allProjectiles.begin() + i);
				j = allEnemies.size();
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
	for (int i = 0; i < allProjectiles.size(); i++)
	{
		target.draw(allProjectiles[i]);
	}

}