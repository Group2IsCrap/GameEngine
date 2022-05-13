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
	, m_radius(3)
{
	sm_biomeMap =
	{
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/yellow.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/red.png"),
		Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/tempbridgge.png")
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

	DrawCircles();

	//Firelight::Maths::Rectf m_sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);
	//Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(-2.0f, -2.0f, 1.0f, 1.0f);
	//int m_layer = 32;
	//double m_rotation = 0.0;

	//for (size_t i = 0; i < 10; ++i)
	//{
	//	Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(-5.0f + (i * 1.5), 3.0f, 1.0f, 1.0f);
	//	Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(m_destinationRect, sm_biomeMap[RandomBiomeIndex(i)], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_sourceRect);
	//}
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

void BiomeGeneration::DrawCircles()
{
	Firelight::Maths::Rectf m_initialSourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, -1.0f, -1.0f);;
	Firelight::Maths::Rectf m_destinationRect = Firelight::Maths::Rectf(0.0f, 3.0f, 1.0f, 1.0f);
	Firelight::Maths::Rectf m_centre = Firelight::Maths::Rectf(-10.0f, 0.0f, 1.0f, 1.0f);
	Firelight::Maths::Rectf m_curIslandCentre = m_centre;
	int m_layer = 32;
	double m_rotation = 0.0;

	size_t numberOfIslands = 6;
	size_t bridgeDimensionX = 3;
	size_t bridgeDimensionY = 2;

	m_radius = 4;

	//for each island
	Firelight::Maths::Rectf newDestRect = Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 1.0f);
	for (size_t iterator = 0; iterator < numberOfIslands; ++iterator)
	{
		for (int x = -m_radius; x <= m_radius; ++x)
		{
			for (int y = -m_radius; y <= m_radius; ++y)
			{
				// if distance is within radius
				newDestRect = Firelight::Maths::Rectf(m_curIslandCentre.x + x, m_curIslandCentre.y + y, 1.0f, 1.0f);
				if (CalculateDistance(newDestRect, m_curIslandCentre) <= m_radius)
				{
					Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(newDestRect, sm_biomeMap[RandomBiomeIndex(iterator)], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_initialSourceRect);
				}
			}
		}

		//draw bridge
		newDestRect.x = m_curIslandCentre.x + m_radius + 1;
		newDestRect.y = m_curIslandCentre.y;
		for (size_t x = 0; x < bridgeDimensionX; ++x)
		{
			for (size_t y = 0; y < bridgeDimensionY; ++y)
			{
				Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(newDestRect, sm_biomeMap[5], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_initialSourceRect);
				newDestRect.y = newDestRect.y + 1;
			}
			newDestRect.x = newDestRect.x + 1;
			newDestRect.y = m_curIslandCentre.y;
		}

		// new island start
		m_curIslandCentre.x = m_curIslandCentre.x + (m_radius * 2) + bridgeDimensionX + 1;
	}

}

size_t BiomeGeneration::CalculateDistance(Firelight::Maths::Rectf rect1, Firelight::Maths::Rectf rect2)
{
	return sqrt(pow(rect1.x - rect2.x, 2) + pow(rect1.y - rect2.y, 2));
}

size_t BiomeGeneration::CalculateIslandShape(int perlinIndex)
{
	float* noiseData = m_islandShapeNoise->GetNoiseData();
	float data = noiseData[perlinIndex];



	return 0;
}