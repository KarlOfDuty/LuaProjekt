#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "collision.h"
#include "LuaScript.h"
#include "Tile.h"

int windowWidth = 1280;
int windowHeight = 720;
sf::Clock deltaTime;

std::vector<sf::CircleShape> allEnemies;

sf::CircleShape test(100,4);
sf::CircleShape test2(80,7);
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

	//Test cube
	test.rotate(45);
	test.setFillColor(sf::Color::White);
	test.setOrigin(test.getRadius(), test.getRadius());
	test.setPosition(700, 200);
	
	//test cube2
	test2.rotate(45);
	test2.setFillColor(sf::Color::White);
	test2.setOrigin(test2.getRadius(), test2.getRadius());
	test2.setPosition(800, windowHeight / 2);

	allEnemies.push_back(test);
	allEnemies.push_back(test2);
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
			window.draw(allEnemies[i]);
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
	player.update(dt,allEnemies);
}