#include "State_Paused.h"
#include "StateManager.h"


State_Paused::State_Paused(StateManager* l_stateManager):BaseState(l_stateManager){}


State_Paused::~State_Paused(){}

void State_Paused::Activate(){}

void State_Paused::Deactivate(){}

void State_Paused::Update(const sf::Time & l_time){}

void State_Paused::Draw(){
	sf::RenderWindow* window = m_stateManager->GetSharedContext()->m_window->GetRenderWindow();

	window->draw(m_rect);
	window->draw(m_text);
}

void State_Paused::OnCreate(){
	std::string str = "Paused";
	SetTransparent(true);
	sf::Vector2u windowSize = m_stateManager->GetSharedContext()->m_window->GetWindowSize();
	m_font.loadFromFile("assets/arial.ttf");

	m_text.setString(str);
	m_text.setFont(m_font);
	sf::FloatRect rect = m_text.getLocalBounds();

	m_text.setOrigin(rect.left+rect.width/2.0,
					 rect.top+rect.height/2.0);
	m_text.setPosition(windowSize.x/2.0f,
					   windowSize.y/2.0f);
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;

	m_rect.setFillColor(sf::Color(0, 0, 0, 150));
	m_rect.setPosition(0, 0);
	m_rect.setSize(sf::Vector2f(windowSize));
	eventManager->AddCallBack(StateType::Paused, "Key_P", &State_Paused::Unpause, this);

}
void State_Paused::OnDestroy(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->RemoveCallBack(StateType::Paused, "Key_P");
}

void State_Paused::Unpause(EventDetails * l_details){
	m_stateManager->SwitchTo(StateType::Game);
}
