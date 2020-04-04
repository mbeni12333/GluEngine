#include "Node.h"

Node::Node(int x, int y):m_pos(x, y){
	m_parent = nullptr;
	g_cost = 0;
	h_cost = 0;
}

Node::~Node(){}

sf::Vector2i Node::GetPos(){
	return m_pos;
}

Node * Node::GetParent(){
	return m_parent;
}

void Node::SetParent(Node * l_node){
	m_parent = l_node;
}

int Node::GetFcost(){
	return g_cost+h_cost;
}

int Node::GetHcost(){
	return h_cost;
}

int Node::GetGcost(){
	return g_cost;
}


void Node::SetHcost(int h){
	h_cost = h;
}

void Node::SetGcost(int g){
	g_cost = g;
}

int Node::GetDistance(Node * a, Node * b){
	int distx = abs(a->GetPos().x-b->GetPos().x);
	int disty = abs(a->GetPos().y-b->GetPos().y);

	if(distx>disty){
		return 14*disty+10*(distx-disty);
	} else{
		return 14*distx+10*(disty-distx);
	}
}


