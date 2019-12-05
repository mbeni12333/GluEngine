#include "Game.h"



Game::Game(): m_window("Moving MushRoom", sf::Vector2u(800, 600)){
	m_mushroomTexture.loadFromFile("assets/zombie.png");
	m_mushroom.setTexture(m_mushroomTexture);
	m_increment = sf::Vector2i(40, 40);
}


Game::~Game(){}

void Game::HandleInput(){}

void Game::Update(){
	m_window.Update();
	MoveMushRoom();
}

void Game::Render(){
	m_window.BeginDraw();
	m_window.Draw(m_mushroom);
	m_window.EndDraw();
}

void Game::RestartClock(){
	m_elpased = m_clock.restart();
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
