#ifndef DOOR_H
#define DOOR_H

#include <SFML\Graphics.hpp>

class Door : public sf::Drawable
{
private:
	sf::RectangleShape shape;
	bool active;
	sf::Vector2f playerNewPos;
	std::string mapName;
public:
	Door(sf::Vector2f pos, sf::Vector2f playerNewPos, std::string mapName);
	~Door();
	sf::RectangleShape& getShape();
	sf::Vector2f getCenterPos();
	sf::Vector2f getPlayerNewPos();
	std::string getMapName();
	bool isActive();
	void setActive(bool active);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};
#endif

