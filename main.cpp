// this file tests if everything is working as expected

#include <SFML/Graphics.hpp>
#include "Game.h"

void main(int argc, char** argv[]) {

	
	Game game;
	sf::Clock clock;
	sf::Time elpased;
	float time_step = 1.0f/60.0f;

	while(!game.GetWindow()->isDone()){

		if(elpased.asSeconds()>=time_step){
			game.Update();
			game.Render();
			game.LateUpdate();
			elpased -= sf::seconds(time_step);
		} else{
			elpased += clock.restart();
		}

		
	}

}
