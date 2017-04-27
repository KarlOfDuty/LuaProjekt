#include<lua.hpp>
#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "collision.h"

int windowWidth = 1280;
int windowHeight = 720;

sf::CircleShape test(100,4);
sf::CircleShape test2(80,7);

void update();

int main()
{
	lua_State* luaState = luaL_newstate();
	luaL_openlibs(luaState);
	luaL_dostring(luaState,"print('Hi.')");
	lua_close(luaState);

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
	test.setOrigin(200 / 2, 200 / 2);
	test.setPosition(500, windowHeight / 2);
	
	//test cube2
	test2.rotate(45);
	test2.setFillColor(sf::Color::White);
	test2.setOrigin(200 / 2, 200 / 2);
	test2.setPosition(800, windowHeight / 2);

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
		window.draw(test);
		window.draw(test2);
		window.display();
	}
	//Release resources...
	return 0;

}

void update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		test.setPosition(test.getPosition().x-3, test.getPosition().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		test.setPosition(test.getPosition().x + 3, test.getPosition().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		test.setPosition(test.getPosition().x, test.getPosition().y-3);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		test.setPosition(test.getPosition().x, test.getPosition().y+3);
	}

	sf::Vector2f mtv;
	if (collision::collides(test, test2, mtv))
	{
		test.setPosition(test.getPosition() + mtv);
	}
}