#include "BaseState.h"
#include "EventManager.h"
#include "SpriteSheet.h"

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
	void scale(EventDetails* l_details);
	void jump(EventDetails* l_details);
	void right(EventDetails *l_details);
	void left(EventDetails *l_details);
private:
	sf::Texture m_spriteTexture;
	sf::Sprite m_sprite;

	bool m_moving;
	bool m_jumping;

	SpriteSheet m_spriteSheet;

};

