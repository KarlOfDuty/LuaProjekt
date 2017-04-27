#include<lua.hpp>
#include<SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int windowWidth = 1280;
int windowHeight = 720;

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
	sf::RectangleShape test(sf::Vector2f(200,200));
	test.setFillColor(sf::Color::White);
	test.setOrigin(200 / 2, 200 / 2);
	test.setPosition(windowWidth / 2, windowHeight / 2);

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
		window.clear();
		window.draw(test);
		window.display();
	}
	//Release resources...
	return 0;

}