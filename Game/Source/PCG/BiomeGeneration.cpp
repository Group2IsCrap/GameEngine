#include "BiomeGeneration.h"
#include "../FirelightEngine/Source/Graphics/GraphicsHandler.h"
#include "../FirelightEngine/Source/Graphics/Data/Texture.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"
#include "../FirelightEngine/Source/Maths/Rect.h"
#include "../FirelightEngine/Source/Graphics/GraphicsEvents.h"
#include "../FirelightEngine/Source/Graphics/SpriteBatch.h"
#include "../FirelightEngine/Source/Events/EventDispatcher.h"

unsigned int BiomeGeneration::mapSeed = 1234;

BiomeGeneration::BiomeGeneration()
	: m_biomeNoise(nullptr)
	, m_islandShapeNoise(nullptr)
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
	m_biomeNoise = new Noise();
	m_biomeNoise->SetSeed(3007);
	m_biomeNoise->SetNoiseScale(250.0f);
	m_biomeNoise->CreateNoise();

	m_islandShapeNoise = new Noise();
	m_islandShapeNoise->SetNoiseScale(1.0f);
	m_islandShapeNoise->CreateNoise();
}

void BiomeGeneration::Uninitialise()
{
	delete m_biomeNoise;
	delete m_islandShapeNoise;
}

void BiomeGeneration::Render()
{
	Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::Graphics::OnEarlyRender>(std::bind(&BiomeGeneration::Draw, this));
}

void BiomeGeneration::Draw()
{
	//TILE MAP DRAWS SO THIS FUNCTION SHOULDNT EXIST


	Firelight::Maths::Rectf m_sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
	Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(-2.0f, -2.0f, 1.0f, 1.0f);
	int m_layer = 32;
	double m_rotation = 0.0;

	for (size_t i = 0; i < 10; ++i)
	{
		Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(-5.0f + (i * 1.5), 3.0f, 1.0f, 1.0f);
		Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(m_destinationRect, sm_biomeMap[RandomBiomeIndex(i)], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_sourceRect);
	}
}

size_t BiomeGeneration::RandomBiomeIndex(unsigned int perlinIndex)
{
	float* noiseData = m_biomeNoise->GetNoiseData();
	float data = noiseData[perlinIndex];

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
	if (data >= 0.6 && data <= 1.0)
	{
		return 4;
	}
}

size_t BiomeGeneration::CalculateIslandShape(int perlinIndex)
{
	float* noiseData = m_islandShapeNoise->GetNoiseData();
	float data = noiseData[perlinIndex];



	return 0;
}