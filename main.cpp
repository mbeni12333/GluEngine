// this file tests if everything is working as expected

#include <SFML/Graphics.hpp>


void main(int argc, char** argv[]) {

	// initialise the main window
	sf::RenderWindow window(sf::VideoMode(640, 480), "First window");


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::Black);
		sf::RectangleShape rectangle(sf::Vector2f(128.0f, 128.0f));
		rectangle.setFillColor(sf::Color::Red);
		window.draw(rectangle);
		window.display();
	}
}
