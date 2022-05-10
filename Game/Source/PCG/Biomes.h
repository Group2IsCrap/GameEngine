#pragma once
#include "../FirelightEngine/Source/Graphics/Data/Texture.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"

struct Biome
{
	Firelight::Graphics::Texture* m_biomeTexture;

	Firelight::Graphics::Texture* m_forestTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png");
	Firelight::Graphics::Texture* m_snowTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png");
	Firelight::Graphics::Texture* m_desertTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/yellow.png");
	Firelight::Graphics::Texture* m_swampTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png");
	Firelight::Graphics::Texture* m_hellscapeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/red.png");
};

struct Forest : public Biome
{
	Firelight::Graphics::Texture* m_biomeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png");
};

struct Snow : public Biome
{
	Firelight::Graphics::Texture* m_biomeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png");
};

struct Desert : public Biome
{
	Firelight::Graphics::Texture* m_biomeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/yellow.png");;
};

struct Swamp : public Biome
{
	Firelight::Graphics::Texture* m_biomeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png");;
};

struct Hellscape : public Biome
{
	Firelight::Graphics::Texture* m_biomeTexture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/red.png");
};