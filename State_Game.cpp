#include "State_Game.h"
#include "StateManager.h"


State_Game::State_Game(StateManager * l_stateManager):BaseState(l_stateManager){}

State_Game::~State_Game(){}

void State_Game::Activate(){}

void State_Game::Deactivate(){}

void State_Game::Update(const sf::Time & l_time){

}

void State_Game::Draw(){
	sf::RenderWindow* window = m_stateManager->GetSharedContext()->m_window->GetRenderWindow();

	window->draw(m_sprite);

}

void State_Game::OnCreate(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->AddCallBack(StateType::Game, "Key_P", &State_Game::Pause, this);
	eventManager->AddCallBack(StateType::Game, "MoveSprite", &State_Game::MoveSprite, this);
	Window* window = m_stateManager->GetSharedContext()->m_window;

	m_spriteTexture.loadFromFile("assets/zombie.png");
	m_sprite.setTexture(m_spriteTexture);
	m_sprite.setOrigin(m_spriteTexture.getSize().x/2,
						 m_spriteTexture.getSize().y/2);
	m_sprite.setPosition(window->GetWindowSize().x/2,
						   window->GetWindowSize().y/2);
	//m_increment = sf::Vector2i(40, 40);

}

void State_Game::OnDestroy(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->RemoveCallBack(StateType::Game, "Key_p");
	eventManager->RemoveCallBack(StateType::Game, "MoveSprite");
}
void State_Game::Pause(EventDetails *l_details){
	m_stateManager->SwitchTo(StateType::Paused);
}

void State_Game::MoveSprite(EventDetails * l_details){
	sf::Vector2i mousepos = l_details->m_mouse;
	m_sprite.setPosition(mousepos.x, mousepos.y);
}
