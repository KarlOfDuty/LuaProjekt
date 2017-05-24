#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

int windowWidth = 1360;
int windowHeight = 1040;

std::vector<sf::RectangleShape> allTiles;

void update(sf::Window &window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    for (size_t y = 0; y < 12; y++) {
        for (size_t x = 0; x < 16; x++) {
            sf::RectangleShape temp(sf::Vector2f(80,80));
            temp.setPosition(x*80, y*80);
            allTiles.push_back(temp);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
                int x = round(sf::Mouse::getPosition(window).x/80);
                int y = round(sf::Mouse::getPosition(window).y/80);
                std::cout << y*16 + x << std::endl;
                allTiles[y*16 + x].setFillColor(sf::Color::Red);

            }
        }

        update(window);

        window.clear();

        for (size_t i = 0; i < allTiles.size(); i++)
        {
            window.draw(allTiles[i]);
        }

        window.display();
    }

    return 0;
}

void update(sf::Window &window)
{

}
