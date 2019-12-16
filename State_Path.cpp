#include "State_Path.h"
#include "StateManager.h"

State_Path::State_Path(StateManager * l_stateManager):BaseState(l_stateManager),
lck(m_mutex, std::defer_lock), m_started(false), doneStep(false),finish(false){
}

void State_Path::OnCreate(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;

	eventManager->AddCallBack(StateType::Game, "start_block", &State_Path::start_block, this);
	eventManager->AddCallBack(StateType::Game, "end_block", &State_Path::end_block, this);
	eventManager->AddCallBack(StateType::Game, "obstacle_block", &State_Path::obstacle_block, this);
	eventManager->AddCallBack(StateType::Game, "start", &State_Path::start, this);
	eventManager->AddCallBack(StateType::Game, "clear_block", &State_Path::clear_block, this);
	eventManager->AddCallBack(StateType::Game, "clear_all", &State_Path::clear_all, this);

	// initialise the board
	sf::Vector2u windowSize = m_stateManager->GetSharedContext()->m_window->GetWindowSize();

	m_blockSize = 32;

	int blocks_x = windowSize.x/m_blockSize;
	int blocks_y = windowSize.y/m_blockSize;

	m_start = sf::Vector2i(0, 0);
	m_end = sf::Vector2i(blocks_x-1, blocks_y-1);

	m_board.resize(blocks_y);

	for(int i = 0; i<blocks_y; ++i){
		m_board[i].resize(blocks_x);

		for(int j = 0; j<blocks_x; ++j){
			m_board[i][j] = BlockType::NeutralBlock;
		}
	}

	sf::Vector2f rectSize(m_blockSize-1, m_blockSize-1);
	m_rect.setSize(rectSize);

	m_board[m_start.y][m_start.x] = BlockType::StartBlock;
	m_board[m_end.y][m_end.x] = BlockType::EndBlock;
	//m_rect.setOrigin(rectSize.x/2, rectSize.y/2);
	graph = new Graph(&m_board, this);
	m_timeStep = 1.0/300.0f;
	m_elapsed = sf::seconds(0);
	

}

void State_Path::OnDestroy(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;

	eventManager->RemoveCallBack(StateType::Game, "start_block");
	eventManager->RemoveCallBack(StateType::Game, "end_block");
	eventManager->RemoveCallBack(StateType::Game, "obstacle_block");
	eventManager->RemoveCallBack(StateType::Game, "start");
	eventManager->RemoveCallBack(StateType::Game, "clear_block");
	eventManager->RemoveCallBack(StateType::Game, "clear_all");
}

void State_Path::Activate(){}

void State_Path::Deactivate(){}

void State_Path::Update(const sf::Time & l_time){
	
	if(m_elapsed.asSeconds() >= m_timeStep){
		if(m_started){
			try{
				lck.lock();
				finish = true;
				step.notify_all();


				while(!doneStep){
					//std::cout<<"stated waiting for finished"<<std::endl;
					finishStep.wait(lck);
					//std::cout<<"finished notified"<<std::endl;
				}
					
				doneStep = false;

				lck.unlock();
			} catch(std::exception e){
				std::cout<<"Exception : "<<e.what()<<std::endl;
			}

		}

		m_elapsed -= sf::seconds(m_timeStep);
	} else{
		m_elapsed += l_time;
	}
	
}

void State_Path::Draw(){
	sf::RenderWindow* window = m_stateManager->GetSharedContext()->m_window->GetRenderWindow();

	for(int i = 0; i<m_board.size(); i++){
		for(int j = 0; j<m_board.size(); ++j){
			//std::cout<<"HOEIN "<<std:: endl;
			SetRectStyle(m_board[i][j]);
			m_rect.setPosition(sf::Vector2f(j*m_blockSize, i*m_blockSize));
			window->draw(m_rect);
		}
	}


}

void State_Path::start(EventDetails * l_details){
	 
	std::cout<<"Start"<<std::endl;
	graph->SetStart(m_start);
	graph->SetEnd(m_end);
	graph->Clear();
	clear_all(nullptr);
	// tart the thread
	m_started = true;
	m_thread = std::thread(&Graph::find_Astar, graph);
	m_thread.detach();
}
void State_Path::stop(){
	if(m_started){
		m_thread.~thread();
		graph->Clear();
		m_started = false;
	}
	clear_all(nullptr);


}
void State_Path::start_block(EventDetails * l_details){

	stop();
	sf::Vector2i mouse_pos = l_details->m_mouse;

	sf::Vector2i grid_pos = Pos2Grid(mouse_pos);
	if(grid_pos==m_end){ return; }
	if(grid_pos.x<0||grid_pos.y<0||
	   grid_pos.x>=m_board.size()||grid_pos.y>=m_board.size())return;
	m_board[grid_pos.y][grid_pos.x] = BlockType::StartBlock;
	if(m_start!=grid_pos){
		m_board[m_start.y][m_start.x] = BlockType::NeutralBlock;
		m_start = grid_pos;
	}

	std::cout<<"start block pos : "<<grid_pos.x<<", "<<grid_pos.y<<std::endl;
}

void State_Path::end_block(EventDetails * l_details){
	stop();
	sf::Vector2i mouse_pos = l_details->m_mouse;

	sf::Vector2i grid_pos = Pos2Grid(mouse_pos);
	if(grid_pos==m_start){ return; }
	if(grid_pos.x<0||grid_pos.y<0||
	   grid_pos.x>=m_board.size()||grid_pos.y>=m_board.size())return;
	m_board[grid_pos.y][grid_pos.x] = BlockType::EndBlock;

	if(m_end!=grid_pos){
		m_board[m_end.y][m_end.x] = BlockType::NeutralBlock;
		m_end = grid_pos;
	}
	std::cout<<"end block pos : " << grid_pos.x << ", " << grid_pos.y <<std::endl;
}

void State_Path::obstacle_block(EventDetails * l_details){
	stop();
	sf::Vector2i mouse_pos = l_details->m_mouse;

	sf::Vector2i grid_pos = Pos2Grid(mouse_pos);

	if(grid_pos==m_start||grid_pos==m_end){ return; }
	if(grid_pos.x<0||grid_pos.y<0||
	   grid_pos.x>=m_board.size()||grid_pos.y>=m_board.size())return;
	m_board[grid_pos.y][grid_pos.x] = BlockType::ObstacleBlock;
	std::cout<<"obstacle block pos : "<<grid_pos.x <<", "<<grid_pos.y<<std::endl;
}

void State_Path::clear_block(EventDetails * l_details){
	sf::Vector2i mouse_pos = l_details->m_mouse;

	sf::Vector2i grid_pos = Pos2Grid(mouse_pos);

	if(grid_pos==m_start||grid_pos==m_end){ return; }
	if(grid_pos.x<0||grid_pos.y<0||
	   grid_pos.x>=m_board.size()||grid_pos.y>=m_board.size())return;
	m_board[grid_pos.y][grid_pos.x] = BlockType::NeutralBlock;
	std::cout<<"clear block pos : "<<grid_pos.x<<", "<<grid_pos.y<<std::endl;
}

void State_Path::clear_all(EventDetails * l_details){
	for(int i = 0; i<m_board.size(); i++){
		for(int j = 0; j<m_board.size(); ++j){
			if(m_board[i][j]==BlockType::ClosedBlock||
				m_board[i][j]==BlockType::OpenBlock||
			   m_board[i][j]==BlockType::PathBlock){

				m_board[i][j] = BlockType::NeutralBlock;
			}
		}
	}
}

void State_Path::SetRectStyle(BlockType l_type){
	switch(l_type){
		case BlockType::NeutralBlock:
			m_rect.setFillColor(sf::Color::White);
			break;
		case BlockType::StartBlock:
			m_rect.setFillColor(sf::Color::Cyan);
			break;
		case BlockType::EndBlock:
			m_rect.setFillColor(sf::Color::Magenta);
			break;
		case BlockType::ObstacleBlock:
			m_rect.setFillColor(sf::Color::Black);
			break;
		case BlockType::OpenBlock:
			m_rect.setFillColor(sf::Color::Green);
			break;
		case BlockType::ClosedBlock:
			m_rect.setFillColor(sf::Color::Red);
			break;
		case BlockType::PathBlock:
			m_rect.setFillColor(sf::Color::Yellow);
			break;

	}
	//m_rect.setFillColor(sf::Color::White);
}

sf::Vector2i State_Path::Pos2Grid(sf::Vector2i l_pos){
	
	return sf::Vector2i(l_pos.x / m_blockSize, l_pos.y / m_blockSize);
}

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
	return g_cost + h_cost;
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

	std::unique_lock<std::mutex> lck(state->m_mutex);
	//state->lck.lock();
	//state->m_mutex.lock();

	Node* startNode = m_boardNodes[m_start.y][m_start.x];
	Node* EndNode = m_boardNodes[m_end.y][m_end.x];

	// initialise
	m_open.emplace_back(startNode);
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
		if(currentNode != startNode && currentNode != EndNode)
		(*m_board)[currentNode->GetPos().y][currentNode->GetPos().x] = BlockType::ClosedBlock;

		state->doneStep = true;
		state->finishStep.notify_all();
	}
	state->m_started = false;
	lck.unlock();
}
void Graph::find_Astar(){

	std::unique_lock<std::mutex> lck(state->m_mutex);
	//state->lck.lock();
	//state->m_mutex.lock();

	Node* startNode = m_boardNodes[m_start.y][m_start.x];
	Node* EndNode = m_boardNodes[m_end.y][m_end.x];

	// initialise
	m_open.emplace_back(startNode);
	state->m_started = true;




	while(!m_open.empty()){
		// get the first node
		while(!state->finish){
			state->step.wait(lck);
		}

		state->finish = false;

		// current Node = the Node with lowest H cost
		auto itr = min_cost();
		Node* currentNode = *itr;
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

	auto itr = m_open.begin();
	auto itr2 = itr;

	for(itr2 = itr; itr2 != m_open.end(); ++itr2){
		if((*itr2)->GetFcost()<(*itr)->GetFcost()||
		   ((*itr2)->GetFcost()==(*itr)->GetFcost() && (*itr2)->GetHcost()<(*itr)->GetHcost())){
			itr = itr2;
		}
	}

	return itr;
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

	if(i==posY && j==posX){ return;}
			
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
	itr = std::find(m_open.begin(), m_open.end(), tmpNode);

	int newCost = l_node->GetGcost()+Node::GetDistance(l_node, tmpNode);
	
	if(itr==m_open.end() || newCost < tmpNode->GetGcost()){

		tmpNode->SetGcost(newCost);
		tmpNode->SetHcost(Node::GetDistance(tmpNode, m_boardNodes[m_end.y][m_end.x]));

		tmpNode->SetParent(l_node);

		if(itr==m_open.end()){
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

	while(tmp != StartNode){
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
