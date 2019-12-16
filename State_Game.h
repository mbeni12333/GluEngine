#include "BaseState.h"
#include "EventManager.h"
#pragma once

class State_Game:
	public BaseState{
public:
	State_Game(StateManager* l_stateManager);
	~State_Game();
	void Activate();
	void Deactivate();
	void Update(const sf::Time& l_time);
	void Draw();
	void OnCreate();
	void OnDestroy();
	void Pause(EventDetails* l_details);
	void MoveSprite(EventDetails* l_details);
private:
	sf::Texture m_spriteTexture;
	sf::Sprite m_sprite;
	sf::Vector2i m_increment;
};

