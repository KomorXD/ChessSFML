#pragma once

#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceHolder {
	private:
		ResourceMananger<sf::Texture>	  textures;
		ResourceMananger<sf::Font>		  fonts;
		ResourceMananger<sf::SoundBuffer> sounds;

		ResourceHolder();

	public:
		void AddTexture(const std::string& name);
		void AddFont(const std::string& name);
		void AddSound(const std::string& name);

		sf::Texture&	 GetTexture(const std::string& name);
		sf::Font&		 GetFont(const std::string& name);
		sf::SoundBuffer& GetSound(const std::string& name);

		void EraseTexture(const std::string& name);
		void EraseFont(const std::string& name);
		void EraseSound(const std::string& name);

		static ResourceHolder& Get();
};