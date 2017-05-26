#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "DoorTile.h"
#include "StaticObject.h"

int windowWidth = 1520;
int windowHeight = 960;

std::vector<Tile*> allTiles;
std::vector<sf::Texture*> allTextures;

Tile* markedTile;
sf::RectangleShape menuBackGround;
sf::RectangleShape menuButtonAdd;
sf::RectangleShape menuButtonSubtract;
sf::RectangleShape editButtonAdd;
sf::RectangleShape editButtonSubtract;
sf::Font font;
sf::Text tileText;
sf::Text tileTextRed;

bool editMapName = false;
bool editTileName = false;
bool editOutDir = false;

void update(sf::RenderWindow &window);
void menuDrawing(sf::RenderWindow &window);
void setCenteredText(std::string text, sf::Text &tileTexture);

int main()
{
	//FONT AND TEXT
	if (!font.loadFromFile("font.ttf"))
	{
		return -1;
	}
	tileText.setFont(font);
	tileText.setString("TESTING TEXT");
	tileText.setFillColor(sf::Color::Green);
	tileText.setCharacterSize(24);
	sf::FloatRect textRect = tileText.getLocalBounds();
	tileText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	tileTextRed.setFont(font);
	tileTextRed.setString("TESTING TEXT");
	tileTextRed.setFillColor(sf::Color::Red);
	tileTextRed.setCharacterSize(24);
	sf::FloatRect textRectRed = tileTextRed.getLocalBounds();
	tileTextRed.setOrigin(textRectRed.left + textRectRed.width / 2.0f, textRectRed.top + textRectRed.height / 2.0f);
	
	//MENU OBJECTS
	sf::Texture* plusTexture = new sf::Texture;
	plusTexture->loadFromFile("textures/plus.png");
	sf::Texture* minusTexture = new sf::Texture;
	minusTexture->loadFromFile("textures/minus.png");
	menuBackGround = sf::RectangleShape(sf::Vector2f(220, 940));
	menuBackGround.setFillColor(sf::Color(90, 90, 90));
	menuBackGround.setPosition(1290, 10);
	menuButtonAdd = sf::RectangleShape(sf::Vector2f(40, 40));
	menuButtonAdd.setOrigin(20, 20);
	menuButtonAdd.setTexture(plusTexture);
	menuButtonAdd.setOutlineColor(sf::Color::Black);
	menuButtonAdd.setOutlineThickness(2);
	menuButtonSubtract = menuButtonAdd;
	menuButtonSubtract.setTexture(minusTexture);

	editButtonAdd = sf::RectangleShape(sf::Vector2f(40, 40));
	editButtonAdd.setOrigin(20, 20);
	editButtonAdd.setTexture(plusTexture);
	editButtonAdd.setOutlineColor(sf::Color::Black);
	editButtonAdd.setOutlineThickness(2);
	editButtonSubtract = editButtonAdd;
	editButtonSubtract.setTexture(minusTexture);
	
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
			allTiles.push_back(new StaticObject(temp,1));
		}
	}

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Editor");
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
					editMapName = false;
					editOutDir = false;
					editTileName = false;
					markedTile->shape.setOutlineThickness(2);
				}
				if (sf::Mouse::getPosition(window).x >= menuButtonSubtract.getPosition().x-20 && sf::Mouse::getPosition(window).x <= menuButtonSubtract.getPosition().x+20 && sf::Mouse::getPosition(window).y >= menuButtonSubtract.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= menuButtonSubtract.getPosition().y + 20)
				{
					StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
					if (staticObject != nullptr)
					{
						staticObject->subtractNumber(allTextures);
					}
				}
				if (sf::Mouse::getPosition(window).x >= menuButtonAdd.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= menuButtonAdd.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= menuButtonAdd.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= menuButtonAdd.getPosition().y + 20)
				{
					StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
					if (staticObject != nullptr)
					{
						staticObject->addNumber(allTextures);
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
							if (sf::Mouse::getPosition(window).x >= editButtonSubtract.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonSubtract.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonSubtract.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonSubtract.getPosition().y + 20)
							{
								if ((door->getMapNum() - 1) != 0)
								{
									door->decreaseMapNum(1);
								}
							}
							if (sf::Mouse::getPosition(window).x >= editButtonAdd.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonAdd.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonAdd.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonAdd.getPosition().y + 20)
							{
								door->increaseMapNum(1);
							}
						}
						else if (editTileName)
						{
							if (sf::Mouse::getPosition(window).x >= editButtonSubtract.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonSubtract.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonSubtract.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonSubtract.getPosition().y + 20)
							{
								door->decreaseTileName();
							}
							if (sf::Mouse::getPosition(window).x >= editButtonAdd.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonAdd.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonAdd.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonAdd.getPosition().y + 20)
							{
								door->increaseTileName();
							}
						}
						else if (editOutDir)
						{
							if (sf::Mouse::getPosition(window).x >= editButtonSubtract.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonSubtract.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonSubtract.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonSubtract.getPosition().y + 20)
							{
								door->decreaseMapDir();
							}
							if (sf::Mouse::getPosition(window).x >= editButtonAdd.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= editButtonAdd.getPosition().x + 20 && sf::Mouse::getPosition(window).y >= editButtonAdd.getPosition().y - 20 && sf::Mouse::getPosition(window).y <= editButtonAdd.getPosition().y + 20)
							{
								door->increaseMapDir();
							}
						}
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::D)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12)
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
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::S)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12)
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
					allTiles[y * 16 + x] = new StaticObject(temp,0);
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.mouseButton.button == sf::Keyboard::A)
			{
				int x = round(sf::Mouse::getPosition(window).x / 80);
				int y = round(sf::Mouse::getPosition(window).y / 80);
				if (x < 16 && y < 12)
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
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
			{
				if (markedTile != nullptr)
				{
					sf::RectangleShape newTile(sf::Vector2f(80, 80));
					newTile.setOutlineColor(sf::Color::Black);
					newTile.setPosition(markedTile->shape.getPosition());
					for (size_t i = 0; i < allTiles.size(); i++)
					{
						if (markedTile == allTiles[i])
						{
							allTiles[i] = new Tile(newTile);
						}
					}
					delete markedTile;
					markedTile = nullptr;
				}
			}
		}

		update(window);

		window.clear();

		window.draw(menuBackGround);

		for (size_t i = 0; i < allTiles.size(); i++)
		{
			window.draw(*allTiles[i]);
		}
		if (markedTile != nullptr)
		{
			window.draw(*markedTile);
			sf::RectangleShape tempShape = markedTile->shape;
			tempShape.setPosition(1360, 80);
			tempShape.setOutlineThickness(0);
			window.draw(tempShape);
		}
		menuDrawing(window);
		window.display();
	}

	return 0;
}

void update(sf::RenderWindow &window)
{
	if (markedTile != nullptr)
	{
		DoorTile* door = dynamic_cast<DoorTile*>(markedTile);
	}
}

void menuDrawing(sf::RenderWindow &window)
{
	if (markedTile != nullptr)
	{
		DoorTile* door = dynamic_cast<DoorTile*>(markedTile);
		StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
		if (door != nullptr)
		{
			setCenteredText("Door", tileText);
			tileText.setPosition(1400, 30);
			window.draw(tileText);
			setCenteredText("Tile name:", tileText);
			tileText.setPosition(1400, 200);
			window.draw(tileText);
			if (!editTileName)
			{
				setCenteredText(door->getTileName(), tileText);
				tileText.setPosition(1400, 250);
				window.draw(tileText);
			}
			else if(editTileName)
			{
				setCenteredText(door->getTileName(), tileTextRed);
				tileTextRed.setPosition(1400, 250);
				window.draw(tileTextRed);
			}
			setCenteredText("Map name:", tileText);
			tileText.setPosition(1400, 300);
			window.draw(tileText);
			if (!editMapName)
			{
				setCenteredText(door->getMapName(), tileText);
				tileText.setPosition(1400, 350);
				window.draw(tileText);
			}
			else if (editMapName)
			{
				setCenteredText(door->getMapName(), tileTextRed);
				tileTextRed.setPosition(1400, 350);
				window.draw(tileTextRed);
			}
			setCenteredText("Out direction:", tileText);
			tileText.setPosition(1400, 400);
			window.draw(tileText);
			if (!editOutDir)
			{
				setCenteredText(door->getOutDirection(), tileText);
				tileText.setPosition(1400, 450);
				window.draw(tileText);
			}
			else if (editOutDir)
			{
				setCenteredText(door->getOutDirection(), tileTextRed);
				tileTextRed.setPosition(1400, 450);
				window.draw(tileTextRed);
			}
			if (editMapName || editOutDir || editTileName)
			{
				editButtonSubtract.setPosition(1350, 500);
				window.draw(editButtonSubtract);
				editButtonAdd.setPosition(1450, 500);
				window.draw(editButtonAdd);
			}
		}
		else if (staticObject != nullptr)
		{
			setCenteredText("Static Object", tileText);
			tileText.setPosition(1400, 30);
			window.draw(tileText);
			setCenteredText("Tile number:", tileText);
			tileText.setPosition(1400, 200);
			window.draw(tileText);
			setCenteredText(std::to_string(staticObject->getTileNumber()), tileText);
			tileText.setPosition(1400, 250);
			window.draw(tileText);
			menuButtonSubtract.setPosition(1350, 250);
			window.draw(menuButtonSubtract);
			menuButtonAdd.setPosition(1450, 250);
			window.draw(menuButtonAdd);
		}
		else
		{
			setCenteredText("Nothing", tileText);
			tileText.setPosition(1400, 30);
			window.draw(tileText);
		}
	}
	else
	{
		setCenteredText("Nothing Marked", tileText);
		tileText.setPosition(1400, 30);
		window.draw(tileText);
	}
}

void setCenteredText(std::string text, sf::Text &tileTexture)
{
		tileTexture.setString(text);
		sf::FloatRect textRect = tileTexture.getLocalBounds();
		tileTexture.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}
