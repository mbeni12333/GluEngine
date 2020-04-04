#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "blocks.h"


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
inline bool operator<=(Node& a, Node& b){
	if(a.GetFcost()<b.GetFcost()||
	   a.GetFcost()==b.GetFcost()&&a.GetHcost()<=b.GetHcost()){
		return true;
	}
	return false;
}


inline bool operator>=(Node& a, Node& b){
	if(a.GetFcost()>b.GetFcost()||
		(a.GetFcost()==b.GetFcost()&&a.GetHcost()>=b.GetHcost())){
		return true;
	}
	return false;
}



