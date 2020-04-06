#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Utilities.h"

class TextureManager: public ResourceManager<TextureManager, sf::Texture>{
public:
	TextureManager():ResourceManager("assets/Textures.cfg"){};

	sf::Texture* Load(const std::string& l_path){
		// allocate memory
		sf::Texture* texture = new sf::Texture();
		if(!texture->loadFromFile(Utils::GetWorkingDirectory()+l_path)){
			// free memory
			delete texture;
			texture = nullptr;

			//print error
			std::cerr<<"!Failed to load Texture : "<<l_path<<std::endl;
		}

		return texture;
	}
};

