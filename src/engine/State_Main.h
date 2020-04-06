#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Main:
	public BaseState{
public:
	State_Main(StateManager* l_stateManager);
	~State_Main();
	void OnCreate();
	void OnDestroy();
	void Update(const sf::Time& l_time);
	void Draw();
	void Activate();
	void Deactivate();

	void MouseLeft(EventDetails *l_details);
private:
	sf::Text m_text;
	sf::Font m_font;
	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;
	sf::RectangleShape m_rect[3];
	sf::Text m_labels[3];
};

