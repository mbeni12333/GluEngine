#include "Game.h"



Game::Game(): m_window("PathFinder", sf::Vector2u(800, 800)), m_stateManager(&m_context),m_textureManager(){

	m_context.m_window = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_eventManager->AddCallBack(StateType(0), "Window_close", &Window::Close, m_context.m_window);
	m_context.m_eventManager->AddCallBack(StateType(0), "FullScreen_toggle", &Window::ToggleFullScreen, m_context.m_window);
	m_stateManager.SwitchTo(StateType::Intro);
	
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
	m_stateManager.Update(m_elpased);
	

	/*float timestep = 1.0/30.0f;

	if(m_elpased.asSeconds()>=timestep){
		

		m_elpased -= sf::seconds(timestep);

		

	}*/
}

void Game::LateUpdate(){
	m_stateManager.ProcessRequests();
	RestartClock();
}

void Game::Render(){
	//float timestep = 1.0/60.0f;

	//if(m_elpased.asSeconds()>=timestep){

		m_window.BeginDraw();
		/*m_world.Render(*m_window.GetRenderWindow());
		m_snake.Render(*m_window.GetRenderWindow());*/
		/*m_window.Draw(m_mushroom);*/
		//if(m_elpased<sf::milliseconds(15))
		
		m_stateManager.Draw();

		//std::cout<<"FPS = "<<1.0f/m_elpased.asSeconds()<<std::endl;
		m_window.EndDraw();
		//m_elpased -= sf::seconds(timestep);
	//} else{
	//	sf::sleep(m_elpased - sf::seconds(timestep));
	//}

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

/*void Game::MoveMushRoom(){
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

}*/
