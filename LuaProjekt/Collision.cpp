#include "collision.h"
#include <iostream>
double PI = 3.1415926535897;

bool collision::collides(sf::CircleShape shape1, sf::CircleShape shape2, sf::Vector2f &mtv)
{
	std::vector<sf::Vector2f> points1 = getPoints(shape1);

	std::vector<sf::Vector2f> points2 = getPoints(shape2);

	float o = 1000000000;
	sf::Vector2f smallestAxis;
	std::vector<sf::Vector2f> axis1 = getAxis(points1);
	std::vector<sf::Vector2f> axis2 = getAxis(points2);

	for (size_t i = 0; i < axis1.size(); i++) {
		sf::Vector2f thisAxis = axis1[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2min > s1max || s2max < s1min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	for (size_t i = 0; i < axis2.size(); i++) {
		sf::Vector2f thisAxis = axis2[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2max < s1min || s1max < s2min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	mtv = smallestAxis*o;
	return true;
}

bool collision::collides(sf::RectangleShape shape1, sf::CircleShape shape2, sf::Vector2f &mtv)
{
	std::vector<sf::Vector2f> points1 = collision::getPoints(shape1);

	std::vector<sf::Vector2f> points2 = collision::getPoints(shape2);

	float o = 1000000000;
	sf::Vector2f smallestAxis;
	std::vector<sf::Vector2f> axis1 = getAxis(points1);
	std::vector<sf::Vector2f> axis2 = getAxis(points2);

	for (size_t i = 0; i < axis1.size(); i++) {
		sf::Vector2f thisAxis = axis1[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2min > s1max || s2max < s1min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	for (size_t i = 0; i < axis2.size(); i++) {
		sf::Vector2f thisAxis = axis2[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2max < s1min || s1max < s2min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	mtv = smallestAxis*o;
	return true;
}

std::vector<sf::Vector2f> collision::getPoints(sf::RectangleShape shape)
{
	std::vector<sf::Vector2f> points;
	float rotationShape = shape.getRotation() * PI / 180;
	sf::Vector2f centerShape = shape.getOrigin() + shape.getPosition();
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		sf::Vector2f point(shape.getPoint(i) + shape.getPosition());
		float x = centerShape.x + (point.x - centerShape.x) * cos(rotationShape) - (point.y - centerShape.y) * sin(rotationShape);
		float y = centerShape.y + (point.x - centerShape.x) * sin(rotationShape) + (point.y - centerShape.y) * cos(rotationShape);
		points.push_back(sf::Vector2f(x, y));
	}
	return points;
}

std::vector<sf::Vector2f> collision::getPoints(sf::CircleShape shape)
{
	std::vector<sf::Vector2f> points;
	float rotationShape1 = shape.getRotation() * PI / 180;
	sf::Vector2f centerShape1 = shape.getPosition();
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		sf::Vector2f point(shape.getPosition() + (shape.getPoint(i)) - sf::Vector2f(shape.getRadius(), shape.getRadius()));
		float x = centerShape1.x + (point.x - centerShape1.x) * cos(rotationShape1) - (point.y - centerShape1.y) * sin(rotationShape1);
		float y = centerShape1.y + (point.x - centerShape1.x) * sin(rotationShape1) + (point.y - centerShape1.y) * cos(rotationShape1);
		points.push_back(sf::Vector2f(x, y));
	}
	return points;
}

std::vector<sf::Vector2f> collision::getAxis(std::vector<sf::Vector2f> allPoints)
{
	std::vector<sf::Vector2f> axis;
	for (size_t i = 0; i < allPoints.size(); i++) {
		sf::Vector2f p1 = allPoints[i];
		sf::Vector2f p2;
		if (i + 1 < allPoints.size())
			p2 = allPoints[i + 1];
		else
			p2 = allPoints[0];

		sf::Vector2f edge = p1 - p2;

		sf::Vector2f normal(-edge.y, edge.x);

		axis.push_back(normalize(normal));
	}
	return axis;
}

void collision::projectOnAxis(std::vector<sf::Vector2f> allPoints, sf::Vector2f thisAxis, float &min, float &max)
{
	min = dot(thisAxis, allPoints[0]);
	max = min;
	for (size_t i = 1; i < allPoints.size(); i++) {
		float p = dot(thisAxis, allPoints[i]);
		if (p < min) min = p;
		else if (p > max) max = p;
	}
}

float collision::dot(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y;
}

sf::Vector2f collision::normalize(sf::Vector2f thisVector)
{
	sf::Vector2f normalized;
	float length = sqrt(pow(thisVector.x, 2) + pow(thisVector.y, 2));
	normalized.x = thisVector.x / length;
	normalized.y = thisVector.y / length;
	return normalized;
}