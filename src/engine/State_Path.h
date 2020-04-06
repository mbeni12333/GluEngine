#pragma once

#include "BaseState.h"
#include "EventManager.h"
#include "../path_finder/blocks.h"

#include "../path_finder/Graph.h"

#include <thread>
#include <mutex>
#include <condition_variable>


class Graph;

class State_Path:
	public BaseState{
	friend class Graph;
public:
	State_Path(StateManager* l_stateManager);
	
	void OnCreate();
	void OnDestroy();
	void Activate();
	void Deactivate();
	void Update(const sf::Time& l_time);
	void Draw();

	void start(EventDetails* l_details);
	void stop();
	void start_block(EventDetails* l_details);
	void end_block(EventDetails* l_details);
	void obstacle_block(EventDetails* l_details);
	void clear_block(EventDetails* l_details);
	void clear_all(EventDetails* l_details);

protected:
	std::thread m_thread;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> lck;
	std::condition_variable step;
	std::condition_variable finishStep;

	bool finish;
	bool doneStep;

private:
	void SetRectStyle(BlockType l_type);
	sf::Vector2i Pos2Grid(sf::Vector2i l_pos);


	sf::RectangleShape m_rect;
	int m_blockSize;

	sf::Vector2i m_start;
	sf::Vector2i m_end;

	bool m_started;
	Board m_board;

	sf::Time m_elapsed;
	float m_timeStep;
	
	Graph* graph;
};

