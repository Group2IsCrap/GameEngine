#include "BiomeGeneration.h"
#include "../FirelightEngine/Source/Graphics/GraphicsHandler.h"
#include "../FirelightEngine/Source/Graphics/Data/Texture.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"
#include "../FirelightEngine/Source/Maths/Rect.h"
#include "../FirelightEngine/Source/Graphics/GraphicsEvents.h"
#include "../FirelightEngine/Source/Graphics/SpriteBatch.h"
#include "../FirelightEngine/Source/Events/EventDispatcher.h"



BiomeGeneration::BiomeGeneration()
	: m_noise(nullptr)
	, m_biome()
{
	sm_biomeMap =
	{
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/yellow.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/red.png")
	};
}

BiomeGeneration* BiomeGeneration::sm_instance = nullptr;

BiomeGeneration* BiomeGeneration::Instance()
{
	if (sm_instance == nullptr)
	{
		sm_instance = new BiomeGeneration();
	}
	return sm_instance;
}

void BiomeGeneration::Initialise()
{
	m_noise = new Noise();
	m_noise->CreateNoise();
}

void BiomeGeneration::Render()
{
	Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::Graphics::OnEarlyRender>(std::bind(&BiomeGeneration::Draw, this));
}

void BiomeGeneration::Draw()
{
	Firelight::Maths::Rectf m_sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
	Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(-2.0f, -2.0f, 1.0f, 1.0f);
	int m_layer = 32;
	double m_rotation = 0.0;

	//For 5 points, draw an image somewhere randomly.

	for (int i = 0; i < 5; ++i)
	{
		Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(m_destinationRect, sm_biomeMap[RandomBiomeIndex(i)], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_sourceRect);
	}
}

size_t BiomeGeneration::RandomBiomeIndex(int perlinIndex)
{
	float data = *(m_noise->GetNoiseData() + perlinIndex);

	if (data >= -1.0 && data <= -0.6)
	{
		return 0;
	}
	if (data > -0.6 && data <= -0.2)
	{
		return 1;
	}
	if (data >= -0.2 && data < 0.2)
	{
		return 2;
	}
	if (data >= 0.2 && data < 0.6)
	{
		return 3;
	}
	if (data >= 6.0 && data <= 1.0)
	{
		return 4;
	}
}