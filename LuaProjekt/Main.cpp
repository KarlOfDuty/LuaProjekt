#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include "Player.h"
#include "collision.h"
#include "LuaScript.h"
#include "Tile.h"
#include "Enemy.h"
#include "StaticObject.h"
#include <crtdbg.h>

int windowWidth = 1280;
int windowHeight = 960;
sf::Clock deltaTime;

//std::vector<Enemy*> allEnemies;
//std::vector<StaticObject*> allStaticObjects;

Player* player;
Tile mapTile;

void update();

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
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

	player = new Player();

	if (!mapTile.loadMap("tiles/finetiles.png", sf::Vector2u(80, 80), "map1", 16, 12))
		return -1;

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
		window.draw(*player);
		window.display();
	}
	//Release resources...
	delete player;
	return 0;
}

void update()
{
	float dt = deltaTime.restart().asSeconds();
	player->update(dt, mapTile.getAllEnemies(), mapTile.allStaticObjects);
	mapTile.update(dt, player);
}