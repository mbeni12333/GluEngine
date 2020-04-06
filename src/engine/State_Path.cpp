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

	m_blockSize = 8;

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
	m_timeStep = 1.0/8000.0f;
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
				/*lck.lock();
				finish = true;
				step.notify_all();


				while(!doneStep){
					//std::cout<<"stated waiting for finished"<<std::endl;
					finishStep.wait(lck);
					//std::cout<<"finished notified"<<std::endl;
				}
					
				doneStep = false;

				lck.unlock();*/
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

