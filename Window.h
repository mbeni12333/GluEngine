#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "EventManager.h"

class EvenManager;

class Window{
	
public:
	Window();
	Window(const std::string& l_title, const sf::Vector2u& l_size);
	~Window();

	void BeginDraw();
	void EndDraw();

	void Update();

	bool isDone();
	bool isFocused();
	EventManager* GetEventManager();
	bool isFullScreen();
	sf::Vector2u GetWindowSize();

	void ToggleFullScreen(EventDetails * l_details);

	void Draw(sf::Drawable& l_drawable);
	sf::RenderWindow* GetRenderWindow();
	void Close(EventDetails* l_event);
	sf::FloatRect GetViewSpace();
private:
	void Setup(const std::string& l_title, const sf::Vector2u& l_size);
	void Destroy();
	void Create();

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	EventManager m_eventManager;
	bool m_isDone;
	bool m_isFocused;
	bool m_isFullScreen;
	

};

