#pragma once
#include "BaseState.h"
#include "EventManager.h"


#include <thread>
#include <mutex>
#include <condition_variable>

enum class BlockType {StartBlock = 0, EndBlock, ObstacleBlock, NeutralBlock, OpenBlock, ClosedBlock, PathBlock};

using Board = std::vector<std::vector<BlockType>>;
class Node{
public:
	Node(int x, int y);
	~Node();

	sf::Vector2i GetPos();
	Node* GetParent();
	void SetParent(Node* l_node);
	int GetFcost();
	int GetHcost();
	int GetGcost();

	void SetHcost(int h);
	void SetGcost(int g);
	static int GetDistance(Node* a, Node* b);
private:
	sf::Vector2i m_pos;
	int g_cost;
	int h_cost;
	Node* m_parent;

};
class State_Path;

class Graph{
public:
	Graph(Board* l_board, State_Path* state);
	~Graph();

	void find();
	void find_Astar();
	std::list<Node*>::iterator min_cost();
	void SetStart(sf::Vector2i l_start);
	void SetEnd(sf::Vector2i l_end);
	void AddChildren(Node* l_node);
	void AddChild(Node* l_node, int i, int j);
	void RetracePath();
	void Clear();

private:
	std::vector<std::vector<Node*>> m_boardNodes;

	std::list<Node*> m_open;
	std::list<Node*> m_closed;

	Board* m_board;
	sf::Vector2i m_start;
	sf::Vector2i m_end;

	State_Path* state;

};
class State_Path:
	public BaseState{
	friend Graph;
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

