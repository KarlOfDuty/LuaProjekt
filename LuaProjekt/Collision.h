#ifndef COLLISION_H
#define COLLISION_H
#include <SFML/Graphics.hpp>
#include <math.h>

namespace collision
{
    bool collides(sf::CircleShape shape1, sf::CircleShape shape2, sf::Vector2f &mtv);
	bool collides(sf::RectangleShape shape1, sf::CircleShape shape2, sf::Vector2f &mtv);
	bool collides(sf::RectangleShape shape1, sf::RectangleShape shape2, sf::Vector2f &mtv);
	std::vector<sf::Vector2f> getPoints(sf::RectangleShape shape);
	std::vector<sf::Vector2f> getPoints(sf::CircleShape shape);
    std::vector<sf::Vector2f> getAxis(std::vector<sf::Vector2f> allPoints);
    void projectOnAxis(std::vector<sf::Vector2f> allPoints, sf::Vector2f thisAxis, float &min, float &max);
    float dot(sf::Vector2f vec1, sf::Vector2f vec2);
    sf::Vector2f normalize(sf::Vector2f theVector);
}

#endif
