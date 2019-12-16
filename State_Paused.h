#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Paused:
	public BaseState{
public:
	State_Paused(StateManager* l_stateManager);
	~State_Paused();

	void Activate();
	void Deactivate();
	void Update(const sf::Time& l_time);
	void Draw();
	void OnCreate();
	void OnDestroy();

	void Unpause(EventDetails* l_details);
private:
	sf::Text m_text;
	sf::Font m_font;
	sf::RectangleShape m_rect;

};

