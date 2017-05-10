#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "collision.h"
#include "LuaScript.h"
#include "Tile.h"
#include "Enemy.h"
#include "StaticObject.h"

int windowWidth = 1280;
int windowHeight = 720;
sf::Clock deltaTime;

std::vector<Enemy*> allEnemies;
std::vector<StaticObject*> allStaticObjects;

Player player;
Tile mapTile;

void update();

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lua", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//Activate the window
	window.setActive(true);

	allEnemies.push_back(new Enemy(100, 4, 20, 2, sf::Vector2f(300,300)));
	allStaticObjects.push_back(new StaticObject(sf::Vector2f(100,100), sf::Vector2f(500,500)));

	if (!mapTile.loadMap("tiles/stone-tiles.png", sf::Vector2u(32, 32), "map", 16, 8))
		return -1;

	mapTile.scale(2, 2);

	//Main loop
	bool running = true;
	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
			{
				//End the program
				running = false;
			}
			//Menu/End program
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
			{
				running = false;
			}
		}

		update();
		window.clear();
		window.draw(mapTile);
		for (int i = 0; i < allEnemies.size(); i++)
		{
			window.draw(*allEnemies[i]);
		}
		for (int i = 0; i < allEnemies.size(); i++)
		{
			window.draw(*allStaticObjects[i]);
		}
		window.draw(player);
		window.display();
	}
	//Release resources...
	return 0;
}

void update()
{
	float dt = deltaTime.restart().asSeconds();
	player.update(dt,allEnemies, allStaticObjects);

	for (int i = 0; i < allEnemies.size(); i++)
	{
		allEnemies[i]->update(dt,allStaticObjects);
	}
}