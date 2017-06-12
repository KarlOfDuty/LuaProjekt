#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Collision.h"
#include "Tile.h"
#include "Enemy.h"
#include "StaticObject.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
int windowWidth = 1280;
int windowHeight = 960;
sf::Clock deltaTime;

Player* player;
Tile mapTile;

lua_State* playerState;
lua_State* enemyState;

std::string AIPath = "Enemy.lua";
bool paused = false;

void update();
void reloadLua();
int main()
{
	enemyState = luaL_newstate();
	luaL_openlibs(enemyState);
	luaL_dofile(enemyState,AIPath.c_str());

	playerState = luaL_newstate();
	luaL_openlibs(playerState);
	luaL_dofile(playerState,"Player.lua");

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
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::E)
			{
				paused = !paused;
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
	lua_close(enemyState);
	return 0;
}

void update()
{
	float dt = deltaTime.restart().asSeconds();
	if (!paused)
	{
		//reloadLua("Player.lua");
		player->update(playerState, dt, mapTile.getAllEnemies(), mapTile.allStaticObjects);
		//reloadLua("AI.lua");
		mapTile.update(enemyState, dt, player);
	}
}

void reloadLua()
{
	lua_close(enemyState);
	enemyState = luaL_newstate();
	luaL_openlibs(enemyState);
	luaL_dofile(enemyState, AIPath.c_str());

	lua_close(playerState);
	playerState = luaL_newstate();
	luaL_openlibs(playerState);
	luaL_dofile(playerState, "Player.lua");
}
