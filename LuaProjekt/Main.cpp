#include<lua.hpp>
#include<SFML/Window.hpp>
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
	sf::Window window(sf::VideoMode(windowWidth, windowHeight), "Lua", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//Activate the window
	window.setActive(true);

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

		//End the current frame (internally swaps the front and back buffers)
		window.display();
	}
	//Release resources...
	return 0;

}