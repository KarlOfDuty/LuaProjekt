#ifndef TILE_H
#define TILE_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "StaticObject.h"
#include "Door.h"
#include "Collision.h"
#include "Player.h"

class Tile : public sf::Drawable, public sf::Transformable
{
private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	std::vector<std::string> tiles;
	std::vector<Enemy*> allEnemies;
	void freeMemory();
public:
	std::vector<StaticObject*> allStaticObjects;
	std::vector<Enemy*>& getAllEnemies();
	std::vector<Door*> allDoors;
	Tile();
	~Tile();
	bool loadMap(const std::string& tileset, sf::Vector2u tileSize, std::string mapName, unsigned int width, unsigned int height);
	void update(float dt, Player* player);
	void draw(sf::RenderTarget &target, sf::RenderStates states)const;
};
#endif // !TILE_H
