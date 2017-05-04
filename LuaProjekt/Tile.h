#ifndef TILE_H
#define TILE_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

class Tile : public sf::Drawable, public sf::Transformable
{
private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	std::vector<int> tiles;
public:
	Tile();
	~Tile();
	bool loadMap(const std::string& tileset, sf::Vector2u tileSize, std::string mapName, unsigned int width, unsigned int height);
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states)const;
};
#endif // !TILE_H
