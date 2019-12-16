#include "BaseState.h"
#include <unordered_map>
#include <functional>
#include "Window.h"
#include "State_Intro.h"
#include "State_Main.h"
#include "State_Game.h"
#include "State_Paused.h"
#include "State_Path.h"

#pragma once

enum class StateType{
	Intro = 1, MainMenu, Game, Paused, GameOver, Credits
};
struct SharedContext{
	SharedContext():m_window(nullptr), m_eventManager(nullptr){}
	Window* m_window;
	EventManager* m_eventManager;
};
using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager{
public:
	StateManager(SharedContext* l_shared);
	~StateManager();

	void Update(const sf::Time& l_time);
	void Draw();

	void ProcessRequests();
	SharedContext* GetSharedContext();
	bool HasState(const StateType& l_type);

	void SwitchTo(const StateType& l_type);
	void Remove(const StateType& l_type);


private:
	void CreateState(const StateType& l_type);
	void RemoveState(const StateType& l_type);

	template<class T>
	void RegisterState(const StateType& l_type){
		m_stateFactory[l_type] = [this]() -> BaseState*{
			return new T(this);
		};
	}

	SharedContext* m_sharedContext;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};

