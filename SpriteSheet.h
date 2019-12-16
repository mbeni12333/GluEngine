#pragma once
#include <unordered_map>
#include "TextureManager.h"
#include "Direction.h"
#include "Anim_Direction.h"
#include "Utilities.h"
#include <string>
#include <fstream>

class Anim_Base;
using Animations = std::unordered_map<std::string, Anim_Base*>;

class SpriteSheet{
public:
	SpriteSheet(TextureManager* l_textureManager);
	~SpriteSheet();

	void CropSprite(const sf::IntRect& l_rect);
	void SetSpriteSize(const sf::Vector2i& l_size);
	sf::Vector2i GetSpriteSize();
	void SetSpritePosition(const sf::Vector2f& l_pos);
	void SetDirection(Direction l_dir);
	Direction GetDirection();
	bool LoadSheet(const std::string& l_file);
	void ReleaseSheet();

	Anim_Base* GetCurrentAnim();

	bool setAnimation(const std::string& l_name,
					  const bool& l_play = false,
					  const bool& l_loop = false);
	void Update(const float& l_dT);
	void Draw(sf::RenderWindow* l_window);
private:

	std::string m_texture;
	sf::Sprite m_sprite;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_spriteScale;
	Direction m_direction;
	std::string m_animationType;
	Animations m_animations;
	Anim_Base* m_currentAnimation;
	TextureManager* m_textureManager;

};

