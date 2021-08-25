#include "ResourceHolder.hpp"

void ResourceHolder::AddTexture(const std::string& name) {
	textures.AddResource(name);
}

void ResourceHolder::AddFont(const std::string& name) {
	fonts.AddResource(name);
}

void ResourceHolder::AddSound(const std::string& name) {
	sounds.AddResource(name);
}

sf::Texture& ResourceHolder::GetTexture(const std::string& name) {
	return textures.Get(name);
}

sf::Font& ResourceHolder::GetFont(const std::string& name) {
	return fonts.Get(name);
}

sf::SoundBuffer& ResourceHolder::GetSound(const std::string& name) {
	return sounds.Get(name);
}

void ResourceHolder::EraseTexture(const std::string& name) {
	textures.EraseResource(name);
}

void ResourceHolder::EraseFont(const std::string& name) {
	fonts.EraseResource(name);
}

void ResourceHolder::EraseSound(const std::string& name) {
	sounds.EraseResource(name);
}

ResourceHolder& ResourceHolder::Get() {
	static ResourceHolder rh;

	return rh;
}

ResourceHolder::ResourceHolder()
	: textures("Textures", "png"), fonts("Fonts", "ttf"), sounds("SFX", "ogg") {

	textures.AddResource("missing");
}