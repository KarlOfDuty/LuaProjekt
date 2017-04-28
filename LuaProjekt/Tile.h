#ifndef TILE_H
#define TILE_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

class Tile : public sf::Drawable
{
private:
	sf::Texture tileTexture;
	sf::Sprite tiles;
	sf::Vector2i map[100][100];
	sf::Vector2i loadCounter = sf::Vector2i(0, 0);

public:
	Tile();
	~Tile();
	void loadMap(std::string mapName);
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states)const;

	void shitDraw(sf::RenderWindow &window);

};
#endif // !TILE_H
