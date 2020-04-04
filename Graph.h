#pragma once

#include "State_Path.h"
#include "Node.h"
#include "Heap.h"

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