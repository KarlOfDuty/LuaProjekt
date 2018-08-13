#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include "DoorTile.h"
#include "StaticObject.h"
#include "Enemy.h"
#include <crtdbg.h>

int windowWidth = 1520;
int windowHeight = 960;

std::vector<Tile*> allTiles;
std::vector<sf::Texture*> allTextures;
std::vector<Enemy*> allEnemies;

Tile* markedTile;
sf::Texture* plusTexture;
sf::Texture* minusTexture;
sf::Texture* saveMapTexture;
sf::RectangleShape menuBackGround;
sf::RectangleShape menuButtonAdd;
sf::RectangleShape menuButtonSubtract;
sf::RectangleShape mapNameAdd;
sf::RectangleShape mapNameSubtract;
sf::RectangleShape saveMapButton;
sf::Font font;
sf::Text tileText;
sf::Text tileTextRed;
int outputMapNr = 0;

bool editMapName = false;
bool editTileName = false;
bool editOutDir = false;
bool editingEnemy = false;

void setupMenu();
void menuDrawing(sf::RenderWindow &window);
void drawTextAt(std::string text, sf::Text &textObject, int xPos, int yPos, sf::RenderWindow &window);
void saveMap();
bool clickedButton(sf::Vector2i mousePos, sf::RectangleShape button);
void freememory();

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	setupMenu();

	//LOADING TEXTURES
	sf::Image tileset;
	if (!tileset.loadFromFile("textures/finetiles.png"))
	{
		return -1;
	}
	for (int i = 0; i < 9; i++)
	{
		int tu = i % (tileset.getSize().x / 80);
		int tv = i / (tileset.getSize().x / 80);
		sf::IntRect tile(tu * 80, tv * 80, 80, 80);
		sf::Texture* textureTile = new sf::Texture;
		textureTile->loadFromImage(tileset, tile);
		allTextures.push_back(textureTile);
	}

	//LOADING NEW MAP TILES
	for (size_t y = 0; y < 12; y++) {
		for (size_t x = 0; x < 16; x++) {
			sf::RectangleShape temp(sf::Vector2f(80, 80));
			temp.setPosition(x * 80, y * 80);
			temp.setOutlineColor(sf::Color::Magenta);
			temp.setTexture(allTextures[1]);
			allTiles.push_back(new StaticObject(temp, 1));
		}
	}

	//Create vector with right amount of positions
	allEnemies = std::vector<Enemy*>(12 * 16);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Editor");
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12)
				{
					if (markedTile != nullptr)
					{
						markedTile->shape.setOutlineThickness(0);
					}
					markedTile = allTiles[y * 16 + x];
					markedTile->shape.setOutlineThickness(2);
					editMapName = false;
					editOutDir = false;
					editTileName = false;
				}
				if (!editingEnemy)
				{
					if (clickedButton(sf::Mouse::getPosition(window), menuButtonSubtract))
					{
						StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
						if (staticObject != nullptr)
						{
							staticObject->subtractNumber(allTextures);
						}
					}
					if (clickedButton(sf::Mouse::getPosition(window), menuButtonAdd))
					{
						StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
						if (staticObject != nullptr)
						{
							staticObject->addNumber(allTextures);
						}
					}
				}

				if (markedTile != nullptr)
				{
					DoorTile* door = dynamic_cast<DoorTile*>(markedTile);
					if (door != nullptr)
					{
						if (sf::Mouse::getPosition(window).x >= 1400 - 20 && sf::Mouse::getPosition(window).x <= 1400 + 20 && sf::Mouse::getPosition(window).y >= 350 - 20 && sf::Mouse::getPosition(window).y <= 350 + 20)
						{
							editMapName = true;
							editOutDir = false;
							editTileName = false;
						}
						else if (sf::Mouse::getPosition(window).x >= 1400 - 20 && sf::Mouse::getPosition(window).x <= 1400 + 20 && sf::Mouse::getPosition(window).y >= 250 - 20 && sf::Mouse::getPosition(window).y <= 250 + 20)
						{
							editTileName = true;
							editMapName = false;
							editOutDir = false;
						}
						else if (sf::Mouse::getPosition(window).x >= 1400 - 20 && sf::Mouse::getPosition(window).x <= 1400 + 20 && sf::Mouse::getPosition(window).y >= 450 - 20 && sf::Mouse::getPosition(window).y <= 450 + 20)
						{
							editOutDir = true;
							editMapName = false;
							editTileName = false;
						}

						if (editMapName)
						{
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonSubtract))
							{
								if ((door->getMapNum() - 1) != 0)
								{
									door->decreaseMapNum(1);
								}
							}
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonAdd))
							{
								door->increaseMapNum(1);
							}
						}
						else if (editTileName)
						{
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonSubtract))
							{
								door->decreaseTileName();
							}
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonAdd))
							{
								door->increaseTileName();
							}
						}
						else if (editOutDir)
						{
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonSubtract))
							{
								door->decreaseMapDir();
							}
							if (clickedButton(sf::Mouse::getPosition(window), menuButtonAdd))
							{
								door->increaseMapDir();
							}
						}
					}

					int pos = -1;
					editingEnemy = false;
					for (size_t i = 0; i < allTiles.size(); i++)
					{
						if (allTiles[i] != nullptr)
						{
							if (allTiles[i] == markedTile)
							{
								pos = i;
							}
						}
					}

					if (pos != -1 && allEnemies[pos] != nullptr)
					{
						editingEnemy = true;
						if (clickedButton(sf::Mouse::getPosition(window), menuButtonSubtract))
						{
							allEnemies[pos]->subtractCorner();
						}
						else if (clickedButton(sf::Mouse::getPosition(window), menuButtonAdd))
						{
							allEnemies[pos]->addCorner();
						}
					}
				}

				if (clickedButton(sf::Mouse::getPosition(window), mapNameAdd))
				{
					outputMapNr++;
				}
				else if (clickedButton(sf::Mouse::getPosition(window), mapNameSubtract))
				{
					if (outputMapNr > 0)
					{
						outputMapNr--;
					}
				}
				else if (clickedButton(sf::Mouse::getPosition(window), saveMapButton))
				{
					saveMap();
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::D)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12 && x >= 0 && y >= 0)
				{
					Tile* oldTile = allTiles[y * 16 + x];
					if (markedTile == oldTile)
					{
						markedTile = nullptr;
					}
					sf::RectangleShape temp(sf::Vector2f(80, 80));
					temp.setPosition(x * 80, y * 80);
					temp.setOutlineColor(sf::Color::Magenta);
					temp.setTexture(allTextures[8]);
					allTiles[y * 16 + x] = new DoorTile(temp);
					delete oldTile;
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::S)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12 && x >= 0 && y >= 0)
				{
					Tile* oldTile = allTiles[y * 16 + x];
					if (markedTile == oldTile)
					{
						markedTile = nullptr;
					}
					sf::RectangleShape temp(sf::Vector2f(80, 80));
					temp.setPosition(x * 80, y * 80);
					temp.setOutlineColor(sf::Color::Magenta);
					temp.setTexture(allTextures[0]);
					allTiles[y * 16 + x] = new StaticObject(temp, 0);
					delete oldTile;
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::A)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				std::cout << x << std::endl;

				if (x < 16 && y < 12 && x >= 0 && y >= 0)
				{
					Tile* oldTile = allTiles[y * 16 + x];
					Enemy* oldEnemy = allEnemies[y * 16 + x];
					if (markedTile == oldTile)
					{
						markedTile = nullptr;
					}
					sf::RectangleShape temp(sf::Vector2f(80, 80));
					temp.setPosition(x * 80, y * 80);
					temp.setOutlineColor(sf::Color::Magenta);
					temp.setTexture(allTextures[1]);
					allTiles[y * 16 + x] = new StaticObject(temp, 1);

					sf::CircleShape shape(10 * 3, 3);
					shape.setOrigin(shape.getRadius(), shape.getRadius());
					shape.rotate(45);
					shape.setPosition(x * 80 + 40, y * 80 + 40);
					shape.setFillColor(sf::Color::Red);
					allEnemies[y * 16 + x] = new Enemy(shape, 3);
					delete oldTile;
					delete oldEnemy;
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
			{
				if (markedTile != nullptr)
				{
					sf::RectangleShape newTile(sf::Vector2f(80, 80));
					newTile.setOutlineColor(sf::Color::Magenta);
					newTile.setPosition(markedTile->shape.getPosition());
					newTile.setTexture(allTextures[1]);

					int position = -1;

					for (size_t i = 0; i < allTiles.size(); i++)
					{
						if (markedTile == allTiles[i])
						{
							allTiles[i] = new StaticObject(newTile, 1);
							position = i;
						}
					}
					delete markedTile;
					markedTile = nullptr;

					if (position != -1)
					{
						if (allEnemies[position] != nullptr)
						{
							delete allEnemies[position];
							allEnemies[position] = nullptr;
						}
					}
				}
			}
		}

		window.clear();

		window.draw(menuBackGround);

		for (size_t i = 0; i < allTiles.size(); i++)
		{
			window.draw(*allTiles[i]);
		}
		if (markedTile != nullptr)
		{
			window.draw(*markedTile);

			if (!editingEnemy)
			{
				sf::RectangleShape tempShape = markedTile->shape;
				tempShape.setPosition(1360, 80);
				tempShape.setOutlineThickness(0);
				window.draw(tempShape);
			}

			int pos = -1;
			for (size_t i = 0; i < allTiles.size(); i++)
			{
				if (allTiles[i] != nullptr)
				{
					if (allTiles[i] == markedTile)
					{
						pos = i;
					}
				}
			}

			if (pos != -1 && allEnemies[pos] != nullptr)
			{
				sf::CircleShape tempEnemyShape = allEnemies[pos]->getShape();
				tempEnemyShape.setPosition(1400, 170);
				tempEnemyShape.setOutlineThickness(0);
				window.draw(tempEnemyShape);
			}
		}
		for (size_t i = 0; i < allEnemies.size(); i++)
		{
			if (allEnemies[i] != nullptr)
			{
				window.draw(*allEnemies[i]);
			}
		}
		menuDrawing(window);
		window.display();
	}

	freememory();

	return 0;
}

void setupMenu()
{
	//FONT AND TEXT
	if (!font.loadFromFile("font.ttf"))
	{
		std::cout << "ERROR LOADING FONT" << std::endl;
	}
	tileText.setFont(font);
	tileText.setFillColor(sf::Color::Green);
	tileText.setCharacterSize(24);

	tileTextRed = tileText;
	tileTextRed.setFillColor(sf::Color::Red);

	//Loading texture for buttons
	plusTexture = new sf::Texture;
	plusTexture->loadFromFile("textures/plus.png");
	minusTexture = new sf::Texture;
	minusTexture->loadFromFile("textures/minus.png");
	saveMapTexture = new sf::Texture;
	saveMapTexture->loadFromFile("textures/saveMap.png");

	//Creating background for menu
	menuBackGround = sf::RectangleShape(sf::Vector2f(220, 940));
	menuBackGround.setFillColor(sf::Color(90, 90, 90));
	menuBackGround.setPosition(1290, 10);

	//Setup all buttons
	menuButtonAdd = sf::RectangleShape(sf::Vector2f(40, 40));
	menuButtonAdd.setOrigin(20, 20);
	menuButtonAdd.setTexture(plusTexture);
	menuButtonAdd.setOutlineColor(sf::Color::Black);
	menuButtonAdd.setOutlineThickness(2);
	menuButtonSubtract = menuButtonAdd;
	menuButtonSubtract.setTexture(minusTexture);

	mapNameAdd = sf::RectangleShape(sf::Vector2f(40, 40));
	mapNameAdd.setOrigin(20, 20);
	mapNameAdd.setTexture(plusTexture);
	mapNameAdd.setOutlineColor(sf::Color::Black);
	mapNameAdd.setOutlineThickness(2);
	mapNameSubtract = mapNameAdd;
	mapNameSubtract.setTexture(minusTexture);

	saveMapButton = sf::RectangleShape(sf::Vector2f(200, 70));
	saveMapButton.setOrigin(100, 35);
	saveMapButton.setTexture(saveMapTexture);
	saveMapButton.setOutlineColor(sf::Color::Black);
	saveMapButton.setOutlineThickness(2);
}

void menuDrawing(sf::RenderWindow &window)
{
	if (markedTile != nullptr)
	{
		DoorTile* door = dynamic_cast<DoorTile*>(markedTile);
		StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
		if (door != nullptr)
		{
			drawTextAt("Door", tileText, 1400, 30, window);
			drawTextAt("Tile name:", tileText, 1400, 200, window);
			if (!editTileName)
			{
				drawTextAt(door->getTileName(), tileText, 1400, 250, window);
			}
			else if (editTileName)
			{
				drawTextAt(door->getTileName(), tileTextRed, 1400, 250, window);
			}
			drawTextAt("Map name:", tileText, 1400, 300, window);
			if (!editMapName)
			{
				drawTextAt(door->getMapName(), tileText, 1400, 350, window);
			}
			else if (editMapName)
			{
				drawTextAt(door->getMapName(), tileTextRed, 1400, 350, window);
			}
			drawTextAt("Out direction:", tileText, 1400, 400, window);
			if (!editOutDir)
			{
				drawTextAt(door->getOutDirection(), tileText, 1400, 450, window);
			}
			else if (editOutDir)
			{
				drawTextAt(door->getOutDirection(), tileTextRed, 1400, 450, window);
			}
			if (editMapName || editOutDir || editTileName)
			{
				menuButtonSubtract.setPosition(1350, 500);
				window.draw(menuButtonSubtract);
				menuButtonAdd.setPosition(1450, 500);
				window.draw(menuButtonAdd);
			}
		}
		else if (staticObject != nullptr)
		{
			if (!editingEnemy)
			{
				drawTextAt("Static Object", tileText, 1400, 30, window);
				drawTextAt("Tile number:", tileText, 1400, 200, window);
				drawTextAt(std::to_string(staticObject->getTileNumber()), tileText, 1400, 250, window);
				menuButtonSubtract.setPosition(1350, 250);
				window.draw(menuButtonSubtract);
				menuButtonAdd.setPosition(1450, 250);
				window.draw(menuButtonAdd);
			}
			if (editingEnemy)
			{
				drawTextAt("Enemy Type", tileText, 1400, 30, window);
				menuButtonSubtract.setPosition(1350, 300);
				window.draw(menuButtonSubtract);
				menuButtonAdd.setPosition(1450, 300);
				window.draw(menuButtonAdd);
			}
		}
		else
		{
			drawTextAt("Nothing", tileText, 1400, 30, window);
		}
	}
	else
	{
		drawTextAt("Nothing Marked", tileText, 1400, 30, window);
	}

	//Save infomation
	drawTextAt("Map Name", tileText, 1400, 700, window);
	drawTextAt("map" + std::to_string(outputMapNr), tileText, 1400, 750, window);
	mapNameAdd.setPosition(1450, 800);
	mapNameSubtract.setPosition(1350, 800);
	window.draw(mapNameAdd);
	window.draw(mapNameSubtract);
	saveMapButton.setPosition(1400, 900);
	window.draw(saveMapButton);
}

void drawTextAt(std::string text, sf::Text &textObject, int xPos, int yPos, sf::RenderWindow &window)
{
	textObject.setString(text);
	sf::FloatRect textRect = textObject.getLocalBounds();
	textObject.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	textObject.setPosition(xPos, yPos);
	window.draw(textObject);
}

bool clickedButton(sf::Vector2i mousePos, sf::RectangleShape button)
{
	int halfWidth = button.getSize().x / 2;
	int halfHeight = button.getSize().x / 2;
	return mousePos.x >= button.getPosition().x - halfWidth &&
		mousePos.x <= button.getPosition().x + halfWidth &&
		mousePos.y >= button.getPosition().y - halfHeight &&
		mousePos.y <= button.getPosition().y + halfHeight;
}

void saveMap()
{
	//Open file with mapname
	std::ofstream outFile;
	outFile.open("tiles/map" + std::to_string(outputMapNr) + ".txt");

	//Find all out tilenames
	std::vector<DoorTile*> usedTileNames;
	for (int i = 0; i < allTiles.size(); i++)
	{
		DoorTile* door = dynamic_cast<DoorTile*>(allTiles[i]);
		if (door != nullptr)
		{
			bool tileNameExists = false;
			for (int j = 0; j < usedTileNames.size() && !tileNameExists; j++)
			{
				if (usedTileNames[j]->getTileName() == door->getTileName())
				{
					tileNameExists = true;
				}
			}
			if (!tileNameExists)
			{
				usedTileNames.push_back(door);
			}
		}
	}

	//Write how many doortypes are in the map
	outFile << usedTileNames.size() << std::endl;

	//Write doortiles information to file
	for (int i = 0; i < usedTileNames.size(); i++)
	{
		outFile << usedTileNames[i]->getTileName() << " " << usedTileNames[i]->getMapName() << " " << usedTileNames[i]->getOutDirection() << std::endl;
	}

	//Write all tiles to file
	for (int y = 0; y < 12; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			outFile << allTiles[y * 16 + x]->getTileOutput() << " ";
		}
		outFile << std::endl;
	}
	outFile << std::endl;

	//Write all enemies to file
	for (int y = 0; y < 12; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (allEnemies[y * 16 + x] != nullptr)
			{
				outFile << allEnemies[y * 16 + x]->getAmountOfCorners() << " ";
			}
			else
			{
				outFile << 0 << " ";
			}
		}
		outFile << std::endl;
	}

	outFile.close();
}

void freememory()
{
	for (int i = 0; i < allEnemies.size(); i++)
	{
		delete allEnemies[i];
	}
	for (int i = 0; i < allTiles.size(); i++)
	{
		delete allTiles[i];
	}
	for (int i = 0; i < allTextures.size(); i++)
	{
		delete allTextures[i];
	}
	delete minusTexture;
	delete plusTexture;
	delete saveMapTexture;
}