#include "Graph.h"


Graph::Graph(Board * l_board, State_Path* l_state){
	m_boardNodes.resize(l_board->size());

	for(int i = 0; i<l_board->size(); ++i){
		m_boardNodes[i].resize((*l_board)[i].size());

		for(int j = 0; j<(*l_board)[i].size(); ++j){
			m_boardNodes[i][j] = new Node(j, i);
		}
	}
	m_board = l_board;
	state = l_state;
}

Graph::~Graph(){
	for(auto itr = m_boardNodes.begin(); itr<m_boardNodes.end(); ++itr){
		for(auto itr2 = itr->begin(); itr2<itr->end(); ++itr2){
			delete *itr2;
		}
		itr->clear();
	}
	m_boardNodes.clear();
}

void Graph::find(){
/*
	std::unique_lock<std::mutex> lck(state->m_mutex);
	//state->lck.lock();
	//state->m_mutex.lock();

	Node* startNode = m_boardNodes[m_start.y][m_start.x];
	Node* EndNode = m_boardNodes[m_end.y][m_end.x];

	// initialise
	m_open.push_back(startNode);
	state->m_started = true;
	while(!m_open.empty()){
		// get the first node
		while(!state->finish){
			state->step.wait(lck);
		}

		state->finish = false;


		Node* currentNode = m_open.front();


		//std::cout<<"CurrentNode : "<<currentNode->GetPos().x<<", "<<currentNode->GetPos().y<<std::endl;
		// if the node is the one we are looking for
		if(currentNode==EndNode){
			std::cout<<"Found it "<<std::endl;
			state->doneStep = true;
			state->finishStep.notify_all();
			RetracePath();
			break;
		}
		// add the childrens
		AddChildren(currentNode);
		m_open.erase(m_open.begin());

		// put the current node in the closed vector
		m_closed.push_back(currentNode);
		if(currentNode!=startNode&&currentNode!=EndNode)
			(*m_board)[currentNode->GetPos().y][currentNode->GetPos().x] = BlockType::ClosedBlock;

		state->doneStep = true;
		state->finishStep.notify_all();
	}
	state->m_started = false;
	lck.unlock();
	*/
}
void Graph::find_Astar(){

	std::unique_lock<std::mutex> lck(state->m_mutex);
	//state->lck.lock();
	//state->m_mutex.lock();

	Node* startNode = m_boardNodes[m_start.y][m_start.x];
	Node* EndNode = m_boardNodes[m_end.y][m_end.x];

	// initialise
	m_open.push_back(startNode);
	state->m_started = true;




	while(!m_open.empty()){
		// get the first node
		/*while(!state->finish){
			state->step.wait(lck);
		}

		state->finish = false;*/ 

		// current Node = the Node with lowest H cost
		auto itr = min_cost();
		Node* currentNode = *itr;
		//Node* currentNode = m_open.GetMin();
		// remove from the set
		m_open.erase(itr);
		//std::cout<<"CurrentNode : "<<currentNode->GetPos().x<<", "<<currentNode->GetPos().y<<std::endl;


		// if the node is the one we are looking for
		if(currentNode==EndNode){
			std::cout<<"Found it "<<std::endl;
			state->doneStep = true;
			state->finishStep.notify_all();
			RetracePath();
			break;
		}



		// add the childrens
		AddChildren(currentNode);




		// put the current node in the closed vector
		m_closed.push_back(currentNode);
		if(currentNode!=startNode&&currentNode!=EndNode)
			(*m_board)[currentNode->GetPos().y][currentNode->GetPos().x] = BlockType::ClosedBlock;

		state->doneStep = true;
		state->finishStep.notify_all();
	}
	state->m_started = false;
	lck.unlock();
}

std::list<Node*>::iterator Graph::min_cost(){
	/**/
	auto itr = m_open.begin();
	auto itr2 = itr;
	
	for(itr2 = itr; itr2!=m_open.end(); ++itr2){
		if((*itr2)->GetFcost()<(*itr)->GetFcost()||
			((*itr2)->GetFcost()==(*itr)->GetFcost()&&(*itr2)->GetHcost()<(*itr)->GetHcost())){
			itr = itr2;
		}
	}
	
	return itr;
	return std::list<Node*>::iterator();
}

void Graph::SetStart(sf::Vector2i l_start){
	m_start = l_start;
}

void Graph::SetEnd(sf::Vector2i l_end){
	m_end = l_end;
}

void Graph::AddChildren(Node * l_node){
	// position
	int posX = l_node->GetPos().x;
	int posY = l_node->GetPos().y;

	/*for(int i = posY-1; i<posY+2; ++i){
		for(int j = posX-1; j<posX+2; ++j){
			// the currentNode
			//std::cout<<"i = "<<i<<", j = "<<j<<std::endl;

		}
	}*/

	AddChild(l_node, posY-1, posX);
	AddChild(l_node, posY, posX+1);
	AddChild(l_node, posY+1, posX);
	AddChild(l_node, posY, posX-1);
	AddChild(l_node, posY-1, posX-1);
	AddChild(l_node, posY-1, posX+1);
	AddChild(l_node, posY+1, posX-1);
	AddChild(l_node, posY+1, posX+1);
}
void Graph::AddChild(Node* l_node, int i, int j){

	int posX = l_node->GetPos().x;
	int posY = l_node->GetPos().y;

	if(i==posY&&j==posX){ return; }

	if(i<0||j<0||j>=m_board->size()||i>=m_board->size()){
		return;
	};

	if((*m_board)[i][j]==BlockType::ObstacleBlock){ return; }

	// check if the node is not already in the open set, or in the closed set
	Node* tmpNode = m_boardNodes[i][j];

	// if the node is in the closed set return
	auto itr = std::find(m_closed.begin(), m_closed.end(), tmpNode);
	if(itr!=m_closed.end()) return;

	// if it's in the open set update the cost
	auto itr2 = std::find(m_open.begin(), m_open.end(), tmpNode);

	int newCost = l_node->GetGcost()+Node::GetDistance(l_node, tmpNode);

	if(itr2==m_open.end()||newCost<tmpNode->GetGcost()){

		tmpNode->SetGcost(newCost);
		tmpNode->SetHcost(Node::GetDistance(tmpNode, m_boardNodes[m_end.y][m_end.x]));

		tmpNode->SetParent(l_node);

		if(itr2==m_open.end()){
			// add to the open set
			m_open.push_back(tmpNode);

			if((*m_board)[i][j]!=BlockType::EndBlock)
				(*m_board)[i][j] = BlockType::OpenBlock;
		}
		return;
	}

	/*std::cout<<"tmpNode is added to open : "
		<<tmpNode->GetPos().x<<
		", "<<tmpNode->GetPos().y<<std::endl;*/


}

void Graph::RetracePath(){
	Node* EndNode = m_boardNodes[m_end.y][m_end.x];
	Node* StartNode = m_boardNodes[m_start.y][m_start.x];

	Node* tmp = EndNode->GetParent();

	while(tmp!=StartNode){
		// color node
		(*m_board)[tmp->GetPos().y][tmp->GetPos().x] = BlockType::PathBlock;

		//move to parent
		tmp = tmp->GetParent();
	}
}

void Graph::Clear(){
	for(int i = 0; i<m_board->size(); i++){
		for(int j = 0; j<m_board->size(); ++j){
			/*if((*m_board)[i][j]==BlockType::ClosedBlock||
			   (*m_board)[i][j]==BlockType::OpenBlock ||
			   (*m_board)[i][j]==BlockType::PathBlock){

				(*m_board)[i][j] = BlockType::NeutralBlock;
			}*/
			m_boardNodes[i][j]->SetParent(nullptr);
		}
	}
	m_open.clear();
	m_closed.clear();
}
