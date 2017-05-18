#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include "Player.h"
#include "collision.h"
#include "Tile.h"
#include "Enemy.h"
#include "StaticObject.h"
#include <crtdbg.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
int windowWidth = 1280;
int windowHeight = 960;
sf::Clock deltaTime;

std::vector<Enemy*> allEnemies;
//std::vector<StaticObject*> allStaticObjects;

Player* player;
Tile mapTile;
lua_State* L;
std::string AIPath = "test.lua";
void update();
void reloadLua();
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L,AIPath.c_str());
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

	allEnemies.push_back(new Enemy(50, 6, 20, 2, sf::Color::Red, sf::Vector2f(200,700)));
	allEnemies.push_back(new Enemy(50, 5, 20, 2, sf::Color::Blue, sf::Vector2f(600, 800)));
	allEnemies.push_back(new Enemy(50, 3, 20, 2, sf::Color::Yellow, sf::Vector2f(100, 400)));
	allEnemies.push_back(new Enemy(50, 8, 20, 2, sf::Color::Magenta, sf::Vector2f(400,300)));

	if (!mapTile.loadMap("tiles/finetiles.png", sf::Vector2u(80, 80), "map", 16, 12))
		return -1;

	//Main loop
	bool running = true;
	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::R)
			{
				reloadLua();
			}
			else if (windowEvent.type == sf::Event::Closed)
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
		window.draw(*player);
		window.display();
	}
	//Release resources...
	delete player;
	for (int i = 0; i < allEnemies.size(); i++)
	{
		delete allEnemies[i];
	}
	return 0;
}

void update()
{
	float dt = deltaTime.restart().asSeconds();
	player->update(dt,allEnemies, mapTile.allStaticObjects);
	for (int i = 0; i < allEnemies.size(); i++)
	{
		allEnemies[i]->update(L, dt, mapTile.allStaticObjects, player);
	}
	mapTile.update(dt, player);
}

void reloadLua()
{
	lua_close(L);
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, AIPath.c_str());
}
