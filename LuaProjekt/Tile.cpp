#include "Tile.h"

Tile::Tile()
{

}

Tile::~Tile()
{

}

void Tile::loadMap(std::string mapName)
{
	std::ifstream openFile("tiles/" + mapName + ".txt");
	if (openFile.is_open())
	{
		std::string tileLocation;
		openFile >> tileLocation;
		tileTexture.loadFromFile("tiles/" + tileLocation);
		tiles.setTexture(tileTexture);
		while (!openFile.eof())
		{
			std::string str;
			openFile >> str;
			char x = str[0], y = str[2];
			if (!isdigit(x) || !isdigit(y))
			{
				map[loadCounter.x][loadCounter.y] = sf::Vector2i(-1, -1);
			}
			else
			{
				map[loadCounter.x][loadCounter.y] = sf::Vector2i(x - '0', y - '0'); //ASCII values
			}
			if(openFile.peek() == '\n')
			{
				loadCounter.x = 0;
				loadCounter.y++;
			}
			else
			{
				loadCounter.x++;
			}
		}
	}
}

void Tile::update(float dt)
{
	for (int i = 0; i < loadCounter.x; i++)
	{
		for (int j = 0; j < loadCounter.y; j++)
		{
			if (map[i][j].x != -1 && map[i][j].y != -1)
			{
				tiles.setPosition(i * 32, j * 32);
				tiles.setTextureRect(sf::IntRect(map[i][j].x * 32, map[i][j].y * 32, 32, 32));
			}
		}
	}
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states)const
{
	target.draw(tiles, states);
}