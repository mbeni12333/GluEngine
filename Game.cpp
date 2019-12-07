#include "Game.h"



Game::Game(): m_window("Sanayak", sf::Vector2u(800, 600)),
m_snake(m_world.GetBlockSize()), m_world(sf::Vector2u(800, 600)){
	m_mushroomTexture.loadFromFile("assets/zombie.png");
	m_mushroom.setTexture(m_mushroomTexture);
	m_mushroom.setOrigin(m_mushroomTexture.getSize().x/2,
						 m_mushroomTexture.getSize().y/2);
	m_mushroom.setPosition(m_window.GetWindowSize().x/2,
						   m_window.GetWindowSize().y/2);
	m_increment = sf::Vector2i(40, 40);
	m_window.GetEventManager()->AddCallBack("Move", &Game::MoveSprite, this);
}


Game::~Game(){}

void Game::HandleInput(){
	/*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
	   && m_snake.GetDirection()!= Direction::Up){
		m_snake.SetDirection(Direction::Down);
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
			  &&m_snake.GetDirection()!=Direction::Right){
		m_snake.SetDirection(Direction::Left);
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
			  &&m_snake.GetDirection()!=Direction::Left){
		m_snake.SetDirection(Direction::Right);
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
			  &&m_snake.GetDirection()!=Direction::Down){
		m_snake.SetDirection(Direction::Up);
	}*/
}

void Game::Update(){
	m_window.Update();
	
	/*float timestep = 1.0/m_snake.GetSpeed();

	if(m_elpased.asSeconds()>=timestep){
		m_snake.Tick();
		m_world.Update(m_snake);
		m_elpased -= sf::seconds(timestep);

		if(m_snake.HasLost()){
			m_snake.Reset();
		}

	}*/
}

void Game::Render(){
	m_window.BeginDraw();
	/*m_world.Render(*m_window.GetRenderWindow());
	m_snake.Render(*m_window.GetRenderWindow());*/
	m_window.Draw(m_mushroom);
	m_window.EndDraw();
}

void Game::RestartClock(){
	m_elpased += m_clock.restart();
}

sf::Time Game::GetElpased(){
	return m_elpased;
}

Window* Game::GetWindow(){
	return &m_window;
}

void Game::MoveMushRoom(){
	sf::Vector2u l_windowSize = m_window.GetWindowSize();
	sf::Vector2u l_textSize = m_mushroomTexture.getSize();


	if((m_mushroom.getPosition().x>(l_windowSize.x-l_textSize.x) && m_increment.x>0)
	 ||(m_mushroom.getPosition().x<0 && m_increment.x <0)){
		m_increment.x = -m_increment.x;
	}
	if((m_mushroom.getPosition().y>(l_windowSize.y-l_textSize.y)&&m_increment.y>0)
	   ||(m_mushroom.getPosition().y<0&&m_increment.y<0)){
		m_increment.y = -m_increment.y;
	}
	float felpased = m_elpased.asSeconds();

	m_mushroom.setPosition(m_mushroom.getPosition().x+(m_increment.x*felpased),
						   m_mushroom.getPosition().y+m_increment.y*felpased);

}
void Game::MoveSprite(EventDetails* l_details){
	sf::Vector2i mousepos = m_window.GetEventManager()->GetMousePos(m_window.GetRenderWindow());
	m_mushroom.setPosition(mousepos.x, mousepos.y);
	std::cout<<"Moving sprite to "<<mousepos.x<<":"<<mousepos.y<<std::endl;

}
