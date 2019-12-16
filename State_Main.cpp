#include "State_Main.h"
#include "StateManager.h"

State_Main::~State_Main(){}

State_Main::State_Main(StateManager * l_stateManager):BaseState(l_stateManager){

}

void State_Main::OnCreate(){
	m_font.loadFromFile("assets/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString("Main Menu");
	m_text.setCharacterSize(24);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left+textRect.width/2.0,
					 textRect.top+textRect.height/2.0);

	m_text.setPosition(400, 100);
	m_buttonSize = sf::Vector2f(300.0f, 64.0f);
	m_buttonPos = sf::Vector2f(400.0f, 300.0f);
	m_buttonPadding = 10;

	std::string str[3];
	str[0] = "PALAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for(int i = 0; i<3; i++){
		// buttonPosition
		sf::Vector2f buttonPos(m_buttonPos.x,
							   m_buttonPos.y+i*(m_buttonSize.y+m_buttonPadding));
		m_rect[i].setSize(m_buttonSize);
		//std::cout<<"Color is : "<<m_rect[i].getFillColor().toInteger()<<std::endl;
		m_rect[i].setFillColor(sf::Color::Transparent);
		m_rect[i].setOutlineColor(sf::Color::White);
		m_rect[i].setOutlineThickness(2);

		// origin at the center
		m_rect[i].setOrigin(m_buttonSize.x/2.0f,
							m_buttonSize.y/2.0f);
		//set position
		m_rect[i].setPosition(buttonPos);
		// button content
		m_labels[i].setFont(m_font);
		m_labels[i].setString(str[i]);
		m_labels[i].setCharacterSize(18);
		// position the labels
		sf::FloatRect rect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(rect.left+rect.width/2.0f,
							  rect.top+rect.height/2.0f);
		m_labels[i].setPosition(buttonPos);

	}
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->AddCallBack(StateType::MainMenu, "MouseLeft", &State_Main::MouseLeft, this);
}

void State_Main::OnDestroy(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->RemoveCallBack(StateType::MainMenu, "MouseLeft");
	std::cout<<"State main destroyed"<<std::endl;
}

void State_Main::Update(const sf::Time & l_time){}

void State_Main::Draw(){
	sf::RenderWindow* window = m_stateManager->GetSharedContext()->m_window->GetRenderWindow();

	window->draw(m_text);
	for(int i = 0; i<3; i++){
		window->draw(m_rect[i]);
		window->draw(m_labels[i]);
	}
}

void State_Main::Activate(){
	
}

void State_Main::Deactivate(){}

void State_Main::MouseLeft(EventDetails *l_details){
	sf::Vector2i mousePos = l_details->m_mouse;

	float halfX = m_buttonSize.x/2.0f;
	float halfY = m_buttonSize.y/2.0f;

	for(int i = 0; i<3; ++i){
		if(mousePos.x>=m_rect[i].getPosition().x-halfX&&
		   mousePos.x<=m_rect[i].getPosition().x+halfX&&
		   mousePos.y>=m_rect[i].getPosition().y-halfY&&
		   mousePos.y<=m_rect[i].getPosition().y+halfY){
			switch(i){
				case 0:
					m_stateManager->SwitchTo(StateType::Game);
					m_stateManager->Remove(StateType::MainMenu);
					std::cout<<"wtf"<<std::endl;
					break;
				case 1:
					// credits 
					break;
				case 2:
					// exit
					m_stateManager->GetSharedContext()->m_window->Close(l_details);
					break;
			}
		}
	}

}
