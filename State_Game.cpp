#include "State_Game.h"
#include "StateManager.h"


State_Game::State_Game(StateManager * l_stateManager):BaseState(l_stateManager),
m_spriteSheet(l_stateManager->GetSharedContext()->m_textureManager){}

State_Game::~State_Game(){}

void State_Game::Activate(){}

void State_Game::Deactivate(){}

void State_Game::Update(const sf::Time & l_time){

	m_spriteSheet.Update(l_time.asSeconds());


	unsigned int dir = (unsigned int )m_spriteSheet.GetDirection();
	int speed = 500;

	if(dir){
		speed *= -1;
	}
	if(m_moving){
		m_spriteSheet.SetSpritePosition(sf::Vector2f(m_spriteSheet.GetPosition().x+speed*l_time.asSeconds(),
										m_spriteSheet.GetPosition().y));
		m_moving = false;
	}
	/*if(m_jumping){
		m_spriteSheet.SetSpritePosition(sf::Vector2f(m_spriteSheet.GetPosition().x,
													 m_spriteSheet.GetPosition().y - speed*l_time.asSeconds()));
		m_jumping = false;
	}*/
	
}

void State_Game::Draw(){
	sf::RenderWindow* window = m_stateManager->GetSharedContext()->m_window->GetRenderWindow();

	m_spriteSheet.Draw(window);

}

void State_Game::OnCreate(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->AddCallBack(StateType::Game, "Key_P", &State_Game::Pause, this);
	eventManager->AddCallBack(StateType::Game, "MoveSprite", &State_Game::MoveSprite, this);
	eventManager->AddCallBack(StateType::Game, "scale", &State_Game::scale, this);
	eventManager->AddCallBack(StateType::Game, "jump", &State_Game::jump, this);
	eventManager->AddCallBack(StateType::Game, "right", &State_Game::right, this);
	eventManager->AddCallBack(StateType::Game, "left", &State_Game::left, this);
	Window* window = m_stateManager->GetSharedContext()->m_window;

	m_stateManager->GetSharedContext()->m_textureManager->RequireResource("zombie");

	m_spriteTexture = *m_stateManager->GetSharedContext()->m_textureManager->GetResource("zombie");
	//m_spriteTexture.loadFromFile("assets/zombie.png");
	m_sprite.setTexture(m_spriteTexture);
	m_sprite.setOrigin(m_spriteTexture.getSize().x/2,
						 m_spriteTexture.getSize().y/2);
	m_sprite.setPosition(window->GetWindowSize().x/2,
						   window->GetWindowSize().y/2);
	m_sprite.scale(0.5, 0.5);
	//m_increment = sf::Vector2i(40, 40);



	m_spriteSheet.LoadSheet("assets/Player.sheet");
	m_spriteSheet.SetSpritePosition(sf::Vector2f(window->GetWindowSize().x/ 2.0,
												 window->GetWindowSize().y/2.0));

	m_spriteSheet.SetSpriteScale(sf::Vector2f(4, 4));

	m_spriteSheet.GetAnim("Idle")->SetLooping(true);
	m_spriteSheet.GetAnim("Idle")->SetFrameTime(1.0/5.0);

}

void State_Game::OnDestroy(){
	EventManager* eventManager = m_stateManager->GetSharedContext()->m_eventManager;
	eventManager->RemoveCallBack(StateType::Game, "Key_p");
	eventManager->RemoveCallBack(StateType::Game, "MoveSprite");
}
void State_Game::Pause(EventDetails *l_details){
	m_stateManager->SwitchTo(StateType::Paused);
}

void State_Game::MoveSprite(EventDetails * l_details){
	sf::Vector2i mousepos = l_details->m_mouse;
	m_sprite.setPosition(mousepos.x, mousepos.y);
}

void State_Game::scale(EventDetails * l_details){
	float s = l_details->m_mouseWheelDelta;
	std::cout<<"S = "<<s<<std::endl;
	if(s<0){
		s = 1 + s * 0.1;
	} else{
		s = 1 + s * 0.1;
	}
	m_sprite.scale(s, s);
}

void State_Game::jump(EventDetails * l_details){
	m_spriteSheet.setAnimation("Jump", true);
	m_jumping = true;
}

void State_Game::left(EventDetails * l_details){
	m_spriteSheet.setAnimation("Walk", true);
	m_spriteSheet.SetDirection(Direction::left);
	m_moving = true;
}

void State_Game::right(EventDetails * l_details){
	m_spriteSheet.setAnimation("Walk", true);
	m_spriteSheet.SetDirection(Direction::Right);
	m_moving = true;
}
