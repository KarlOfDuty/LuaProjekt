#include "Tile.h"

Tile::Tile()
{

}

Tile::~Tile()
{
	freeMemory();
}

void Tile::freeMemory()
{
	for (int i = 0; i < allStaticObjects.size(); i++)
	{
		delete allStaticObjects[i];
	}
	for (int i = 0; i < allDoors.size(); i++)
	{
		delete allDoors[i];
	}
}

bool Tile::loadMap(const std::string& tileset, sf::Vector2u tileSize, std::string mapName, unsigned int width, unsigned int height)
{
	freeMemory();
	allStaticObjects.clear();
	allDoors.clear();
	tiles.clear();
	std::ifstream openFile;
	openFile.open("tiles/" + mapName + ".txt");
	int num;
	while (openFile >> num)
	{
		tiles.push_back(num);
	}
	openFile.close();

		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
		{
			for (unsigned int j = 0; j < height; ++j)
			{
				// get the current tile number
				int tileNumber = tiles[i + j * width];

				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

				//Create staticobjects and doors
				if (tileNumber == 0)
				{
					allStaticObjects.push_back(new StaticObject(sf::Vector2f(quad[0].position)));
				}
				if (tileNumber == 2)
				{
					allDoors.push_back(new Door(quad[0].position,sf::Vector2f(600,400), "map2"));
				}
			}
		}

	return true;
}

void Tile::update(float dt, Player* player)
{
	for (int i = 0; i < allDoors.size(); i++)
	{
		if (allDoors[i]->isActive())
		{
			sf::Vector2f distanceVector = player->getShape().getPosition() - allDoors[i]->getCenterPos();
			float length = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
			if (length < 110)
			{
				if (collision::collides(allDoors[i]->getShape(), player->getShape(), sf::Vector2f()))
				{
					loadMap("tiles/finetiles.png", sf::Vector2u(80, 80), allDoors[i]->getMapName(), 16, 12);
					player->setPos(allDoors[i]->getPlayerNewPos());
					i = allDoors.size();
				}
			}
		}
	}
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states)const
{

	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);

	for (int i = 0; i < allStaticObjects.size(); i++)
	{
		//target.draw(*allStaticObjects[i]);
	}
}
