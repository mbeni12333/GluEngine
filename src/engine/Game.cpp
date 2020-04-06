#include "Game.h"



Game::Game(std::string game_name): m_window(game_name, sf::Vector2u(800, 800)), m_stateManager(&m_context),m_textureManager(){

	m_context.m_window = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_eventManager->AddCallBack(StateType(0), "Window_close", &Window::Close, m_context.m_window);
	m_context.m_eventManager->AddCallBack(StateType(0), "FullScreen_toggle", &Window::ToggleFullScreen, m_context.m_window);
	m_stateManager.SwitchTo(StateType::Intro);
	
}


Game::~Game(){}


void Game::HandleInput(){}

void Game::Update(){

	m_window.Update();
	m_stateManager.Update(m_elpased);
}

void Game::LateUpdate(){
	m_stateManager.ProcessRequests();
	RestartClock();
}

void Game::Render(){

	m_window.BeginDraw();
		
	m_stateManager.Draw();

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

