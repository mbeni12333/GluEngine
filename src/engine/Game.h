#include "Window.h"
#include "StateManager.h"
#pragma once
class Game{
public:
	Game(std::string game_name);
	~Game();


	void HandleInput();
	void Update();
	void LateUpdate();
	void Render();
	void RestartClock();
	sf::Time GetElpased();
	Window* GetWindow();
private:

	Window m_window;
	sf::Clock m_clock;
	sf::Time m_elpased;
	SharedContext m_context;
	StateManager m_stateManager;
	TextureManager m_textureManager;
};

