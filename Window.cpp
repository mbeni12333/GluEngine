#include "Window.h"



Window::Window(){
	Setup("window", sf::Vector2u(640, 480));
}

Window::Window(const std::string & l_title, const sf::Vector2u & l_size){
	Setup(l_title, l_size);
}


Window::~Window(){
	Destroy();
}

void Window::BeginDraw(){
	m_window.clear(sf::Color::Black);
}

void Window::EndDraw(){
	m_window.display();
}

void Window::Update(){
	sf::Event event;
	while(m_window.pollEvent(event)){
		if(event.type == sf::Event::LostFocus){
			m_isFocused = false;
			m_eventManager.setFocus(false);
			//m_isDone = true;
		}else if(event.type == sf::Event::GainedFocus){
			m_isFocused = true;
			m_eventManager.setFocus(true);
		}

		m_eventManager.HandleEvent(event);
	}
	m_eventManager.Update();
}

bool Window::isDone(){
	return m_isDone;
}

bool Window::isFocused(){
	return false;
}

EventManager * Window::GetEventManager(){
	return &m_eventManager;
}

bool Window::isFullScreen(){
	return m_isFullScreen;
}

sf::Vector2u Window::GetWindowSize(){
	return m_windowSize;
}

void Window::ToggleFullScreen(EventDetails* l_details){
	m_isFullScreen = !m_isFullScreen;
	Destroy();
	Create();
}

void Window::Draw(sf::Drawable & l_drawable){
	m_window.draw(l_drawable);
}

sf::RenderWindow* Window::GetRenderWindow(){
	return &m_window;
}

void Window::Close(EventDetails* l_event){ m_isDone = true; }

sf::FloatRect Window::GetViewSpace(){
	sf::Vector2f viewCenter = m_window.getView().getCenter();
	sf::Vector2f viewSize = m_window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x/2.0, viewSize.y/2.0);
	sf::FloatRect viewSpace(viewCenter-viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::Setup(const std::string & l_title, const sf::Vector2u & l_size){
	m_windowTitle = l_title;
	m_windowSize = l_size;
	m_isDone = false;
	m_isFullScreen = false;
	m_isFocused = true;

	/*m_eventManager.AddCallBack("FullScreen_toggle", &Window::ToggleFullScreen, this);
	m_eventManager.AddCallBack("Window_close", &Window::Close, this);*/
	//m_window.setFramerateLimit(60);
	//m_window.setVerticalSyncEnabled(true);
	Create();
	m_eventManager.SetWindow(&m_window);
}

void Window::Destroy(){}

void Window::Create(){
	auto style = (m_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.create({m_windowSize.x, m_windowSize.y, 32}, m_windowTitle, style);
}
