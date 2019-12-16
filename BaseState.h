#include <SFML/Graphics.hpp>
#pragma once

class StateManager;

class BaseState{
	friend class StateManager;
public:
	BaseState(StateManager* l_stateManager):m_stateManager(l_stateManager),
		m_transparent(false),
		m_transcendent(false){}

	virtual ~BaseState(){}
	
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& l_time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& l_transparent){
		m_transparent = l_transparent;
	}
	bool isTransparent() const{
		return m_transparent;
	}
	void SetTranscendent(const bool& l_transcendent){
		m_transcendent = l_transcendent;
	}
	bool isTranscendent() const{
		return m_transcendent;
	}
	StateManager* GetStateManager() const{
		return m_stateManager;
	}
	sf::View& GetView(){
		return m_view;
	}

protected:
	StateManager* m_stateManager;
	sf::View m_view;
	bool m_transparent;
	bool m_transcendent;
};

