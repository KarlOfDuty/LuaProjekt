#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "DoorTile.h"

int windowWidth = 1520;
int windowHeight = 960;

std::vector<Tile*> allTiles;

Tile* markedTile;
sf::RectangleShape menuBackGround;
sf::Font font;
sf::Text tileText;

void update(sf::RenderWindow &window);
void textDrawing(sf::RenderWindow &window);
void setCenteredText(std::string text);

int main()
{
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

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Editor");

	menuBackGround = sf::RectangleShape(sf::Vector2f(220, 940));
	menuBackGround.setFillColor(sf::Color(90, 90, 90));
	menuBackGround.setPosition(1290, 10);

	for (size_t y = 0; y < 12; y++) {
		for (size_t x = 0; x < 16; x++) {
			sf::RectangleShape temp(sf::Vector2f(80, 80));
			temp.setPosition(x * 80, y * 80);
			temp.setOutlineColor(sf::Color::Black);
			allTiles.push_back(new DoorTile(temp));
		}
	}

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
			window.draw(tempShape);
		}
		textDrawing(window);
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

void textDrawing(sf::RenderWindow &window)
{
	if (markedTile != nullptr)
	{
		DoorTile* door = dynamic_cast<DoorTile*>(markedTile);
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
		else
		{
			setCenteredText("Static Object");
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
