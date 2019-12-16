#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(TextureManager * l_textureManager):
	m_textureManager(l_textureManager),
	m_currentAnimation(nullptr),
	m_spriteScale(1.f, 1.f),
	m_direction(Direction::Right){}

SpriteSheet::~SpriteSheet(){ ReleaseSheet(); }

void SpriteSheet::CropSprite(const sf::IntRect & l_rect){
	m_sprite.setTextureRect(l_rect);
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i & l_size){
	m_spriteSize = l_size;
	// correct the origin
	m_sprite.setOrigin(m_spriteSize.x/2.0f, m_spriteSize.y/2.0f);
}

sf::Vector2i SpriteSheet::GetSpriteSize(){
	return m_spriteSize;
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f & l_pos){
	m_sprite.setPosition(l_pos);
}

void SpriteSheet::SetDirection(Direction l_dir){
	if(m_direction==l_dir){ return; }

	m_direction = l_dir;
	m_currentAnimation->CropSprite();
}

Direction SpriteSheet::GetDirection(){
	return m_direction;
}

bool SpriteSheet::LoadSheet(const std::string & l_file){
	
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory()+l_file);
	
	if(sheet.is_open()){
		// release currentSheet
		ReleaseSheet();
		std::string line;
		
		while(std::getline(sheet, line)){
			// if comment skip
			if(line[0] == '|' ){ continue; }

			std::stringstream keystream(line);

			std::string type;
			keystream>>type;

			if(type=="Texture"){
				if(m_texture!=""){
					std::cerr<<"! Duplicate texture file : "<< l_file <<std::endl;
					continue;
				}
				std::string texture;
				keystream>>texture;

				if(!m_textureManager->RequireResource(texture)){
					std::cerr<<"! Could not setup texture "<<texture<<std::endl;
					continue;
				}

				m_texture = texture;
				m_sprite.setTexture(*m_textureManager->GetResource(m_texture));

			} else if(type=="Size"){
				keystream>>m_spriteSize.x>>m_spriteSize.y;
				SetSpriteSize(m_spriteSize);
				
			} else if(type=="Scale"){
				keystream>>m_spriteScale.x>>m_spriteScale.y;
				m_sprite.setScale(m_spriteScale);

			} else if(type=="AnimationType"){
				keystream>>m_animationType;

			}else if(type == "Animation"){
				std::string name;
				keystream>>name;

				if(m_animations.find(name) != m_animations.end()){
					std::cerr<<"! Duplicate animation name: "<<name<<" in file "<<l_file<<std::endl;
					continue;
				}

				Anim_Base* anim = nullptr;

				if(m_animationType=="Directional"){
					anim = new Anim_Direction();
				} else{
					std::cerr<<"! Uknown animation type : "<<m_animationType<<std::endl;
					continue;
				}

				keystream>>*anim;

				anim->SetSpriteSheet(this);
				anim->SetName(name);
				anim->Reset();
				m_animations.emplace(name, anim);

				if(m_currentAnimation){ continue; }

				m_currentAnimation = anim;
				m_currentAnimation->Play();
			}
		}
		sheet.close();
		return true;
	}
	std::cerr<<"!Failed loading Sprite sheet from file "<<l_file<<std::endl;
	return false;
}

void SpriteSheet::ReleaseSheet(){
	// release the texture
	m_textureManager->ReleaseResource(m_texture);
	// release animations
	while(m_animations.begin()!=m_animations.end()){
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}

}

Anim_Base * SpriteSheet::GetCurrentAnim(){
	return m_currentAnimation;
}

bool SpriteSheet::setAnimation(const std::string & l_name,
							   const bool & l_play,
							   const bool & l_loop){
	// find the animation in m_animations

	auto itr = m_animations.find(l_name);
	if(itr==m_animations.end()){ return false; }
	if(m_currentAnimation==itr->second){ return false; }
	if(m_currentAnimation){ m_currentAnimation->Stop();}
	m_currentAnimation = itr->second;
	m_currentAnimation->SetLooping(l_loop);
	if(l_play){ m_currentAnimation->Play(); }
	m_currentAnimation->CropSprite();
	return true;
}

void SpriteSheet::Update(const float & l_dT){
	m_currentAnimation->Update(l_dT);
}

void SpriteSheet::Draw(sf::RenderWindow * l_window){
	l_window->draw(m_sprite);
}
