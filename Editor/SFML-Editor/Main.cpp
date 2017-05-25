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
sf::Font font;
sf::Text tileText;

void update(sf::RenderWindow &window);
void menuDrawing(sf::RenderWindow &window);
void setCenteredText(std::string text);

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
					markedTile->shape.setOutlineThickness(2);
				}
				if (sf::Mouse::getPosition(window).x >= menuButtonSubtract.getPosition().x-20 && sf::Mouse::getPosition(window).x <= menuButtonSubtract.getPosition().x+20)
				{
					StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
					if (staticObject != nullptr)
					{
						staticObject->subtractNumber(allTextures);
					}
				}
				if (sf::Mouse::getPosition(window).x >= menuButtonAdd.getPosition().x - 20 && sf::Mouse::getPosition(window).x <= menuButtonAdd.getPosition().x + 20)
				{
					StaticObject* staticObject = dynamic_cast<StaticObject*>(markedTile);
					if (staticObject != nullptr)
					{
						staticObject->addNumber(allTextures);
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
			setCenteredText("Door");
			tileText.setPosition(1400, 30);
			window.draw(tileText);
			setCenteredText("Tile name:");
			tileText.setPosition(1400, 200);
			window.draw(tileText);
			setCenteredText(door->getTileName());
			tileText.setPosition(1400, 250);
			window.draw(tileText);
			setCenteredText("Map name:");
			tileText.setPosition(1400, 300);
			window.draw(tileText);
			setCenteredText(door->getMapName());
			tileText.setPosition(1400, 350);
			window.draw(tileText);
			setCenteredText("Out direction:");
			tileText.setPosition(1400, 400);
			window.draw(tileText);
			setCenteredText(door->getOutDirection());
			tileText.setPosition(1400, 450);
			window.draw(tileText);
		}
		else if (staticObject != nullptr)
		{
			setCenteredText("Static Object");
			tileText.setPosition(1400, 30);
			window.draw(tileText);
			setCenteredText("Tile number:");
			tileText.setPosition(1400, 200);
			window.draw(tileText);
			setCenteredText(std::to_string(staticObject->getTileNumber()));
			tileText.setPosition(1400, 250);
			window.draw(tileText);
			menuButtonSubtract.setPosition(1350, 250);
			window.draw(menuButtonSubtract);
			menuButtonAdd.setPosition(1450, 250);
			window.draw(menuButtonAdd);
		}
		else
		{
			setCenteredText("Nothing");
			tileText.setPosition(1400, 30);
			window.draw(tileText);
		}
	}
	else
	{
		setCenteredText("Nothing Marked");
		tileText.setPosition(1400, 30);
		window.draw(tileText);
	}
}

void setCenteredText(std::string text)
{
	tileText.setString(text);
	sf::FloatRect textRect = tileText.getLocalBounds();
	tileText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}
