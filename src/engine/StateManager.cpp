#include "StateManager.h"

StateManager::StateManager(SharedContext * l_shared):m_sharedContext(l_shared){

    /*Replace with automatic state registration from file*/
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_Main>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);

}

StateManager::~StateManager(){
	for(auto& itr: m_states){
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Update(const sf::Time & l_time){
	if(m_states.empty()){ return; }
	if(m_states.back().second->isTranscendent()
	   &&m_states.size()>1){
		auto itr = m_states.end();

		while(itr!=m_states.begin()){
			if(itr!=m_states.end()){
				if(!itr->second->isTranscendent()){
					break;
				}
			}
			--itr;
		}

		for(; itr!=m_states.end(); itr++){
			itr->second->Update(l_time);
		}
	} else{
		m_states.back().second->Update(l_time);
	}
}

void StateManager::Draw(){
	if(m_states.empty()){ return; }
	if(m_states.back().second->isTransparent()
	   && m_states.size()>1){
		// if tranparent, find the first non transparent state
		// or till m_states.begin()
		auto itr = m_states.end();
		while(itr!=m_states.begin()){
			if(itr!=m_states.end()){
				if(!itr->second->isTransparent()){
					break;
				}
			}
			--itr;
		}

		// draw backwards from itr to end
		for(; itr!=m_states.end(); itr++){
			// set the view to the state's view
			m_sharedContext->m_window->GetRenderWindow()->setView(itr->second->GetView());
			// draw the scene
			itr->second->Draw();
		}
	} else{
		m_states.back().second->Draw();
	}
}

void StateManager::ProcessRequests(){
	while(m_toRemove.begin()!=m_toRemove.end()){
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

SharedContext * StateManager::GetSharedContext(){
	return m_sharedContext;
}

bool StateManager::HasState(const StateType & l_type){
	for(auto itr = m_states.begin(); itr!=m_states.end(); itr++){
		if(itr->first==l_type){
			// if we found the type we check if it's not in the removed vector
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
			if(removed==m_toRemove.end()){
				return true;
			}
			return false;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType & l_type){
	m_sharedContext->m_eventManager->SetCurrentState(l_type);
	std::cout<<"Current state = "<<(int)l_type<<std::endl;
	for(auto itr = m_states.begin(); itr!=m_states.end(); itr++){
		if(itr->first==l_type){
			// found it
			// deactivate first state
			m_states.back().second->Deactivate();
			// create temp for the new state
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			// set the view to be the currentState's view
			m_sharedContext->m_window->GetRenderWindow()->setView(tmp_state->GetView());
			// remove the state from it's old position of the stack
			m_states.erase(itr);
			// put it on top
			m_states.emplace_back(tmp_type, tmp_state);
			// activate it
			tmp_state->Activate();
			return;
		}
	}
	// the type is not found, create it
	if(!m_states.empty()){
		m_states.back().second->Deactivate();
	}
	CreateState(l_type);
	if(!m_states.empty()){
		//activate the state we just added
		m_states.back().second->Activate();
		// set the view to the current state's view
		m_sharedContext->m_window->GetRenderWindow()->setView(m_states.back().second->GetView());
	}

	

}

void StateManager::Remove(const StateType & l_type){
	// add the l_type to the remove queue 
	m_toRemove.push_back(l_type);
}

void StateManager::CreateState(const StateType & l_type){
	// check if l_type is defined
	auto newState = m_stateFactory.find(l_type);
	if(newState==m_stateFactory.end()){ return; }

	// create the instance
	BaseState* state = newState->second();
	// set the default view
	state->m_view = m_sharedContext->m_window->GetRenderWindow()->getDefaultView();
	// add to the stack
	m_states.emplace_back(l_type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType & l_type){
	for(auto itr = m_states.begin(); itr!=m_states.end(); itr++){
		if(itr->first==l_type){
			itr->second->OnDestroy();
			delete itr->second;
			m_states.erase(itr);
			return;
		}
	}
}
