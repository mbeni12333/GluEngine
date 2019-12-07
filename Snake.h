#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct SnakeSegment{
	SnakeSegment(int x, int y): position(x, y){}
	sf::Vector2i position;
};
enum Direction {None, Up, Down, Left, Right};
using SnakeContainer = std::vector<SnakeSegment>;


class Snake{
public:
	Snake(int l_blockSize);
	~Snake();


	// helper methods
	void SetDirection(Direction l_dir);
	int GetSpeed();
	sf::Vector2i GetPosition();
	Direction GetDirection();
	int GetScore();
	int GetLives();
	void IncreaseScore();
	bool HasLost();

	void Lose();
	void ToggleLose();

	void Extend();
	void Reset();

	void Move();
	void Tick();
	void Cut(int l_segment);
	void Render(sf::RenderWindow& l_window);

private:
	void CheckCollision();
	SnakeContainer m_snakeBody;
	int m_size;
	int m_lives;
	int m_score;
	int m_speed;
	Direction m_dir;
	bool m_lost;

	sf::RectangleShape m_bodyRect; 
};

