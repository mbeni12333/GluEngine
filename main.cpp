// this file tests if everything is working as expected

#include <SFML/Graphics.hpp>
#include "Game.h"

void main(int argc, char** argv[]) {

	
	Game game;

	while(!game.GetWindow()->isDone()){

		game.Update();
		game.Render();
		game.LateUpdate();
		
	}

}
