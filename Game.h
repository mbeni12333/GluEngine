#include "Window.h"

#pragma once
class Game{
public:
	Game();
	~Game();


	void HandleInput();
	void Update();
	void Render();
	void RestartClock();
	sf::Time GetElpased();
	Window* GetWindow();

private:
	void MoveMushRoom();
	Window m_window;
	sf::Texture m_mushroomTexture;
	sf::Sprite m_mushroom;
	sf::Vector2i m_increment;
	sf::Clock m_clock;
	sf::Time m_elpased;
};

