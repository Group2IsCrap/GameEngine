#include "BiomeGeneration.h"
#include "../FirelightEngine/Source/Graphics/GraphicsHandler.h"
#include "../FirelightEngine/Source/Graphics/Data/Texture.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"
#include "../FirelightEngine/Source/Maths/Rect.h"
#include "../FirelightEngine/Source/Graphics/GraphicsEvents.h"
#include "../FirelightEngine/Source/Graphics/SpriteBatch.h"
#include "../FirelightEngine/Source/Events/EventDispatcher.h"
#include "../FirelightEngine/Source/ECS/Systems/TilemapSystem.h"
#include "BiomeInfo.h"

using namespace Firelight::Events;

unsigned int BiomeGeneration::mapSeed = 1234;

BiomeGeneration::BiomeGeneration()
	: m_biomeNoise(nullptr)
	, m_islandDirectionNoise(nullptr)
	, m_islandShapeNoise(nullptr)
	, m_biome()
	, m_bridgeWidth(2)
	, m_bridgeLength(3)
	, m_radius(3)
{
	sm_biomeMap =
	{
		AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png"),
		AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png"),
		AssetManager::Instance().GetTexture("$ENGINE/Textures/yellow.png"),
		AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png"),
		AssetManager::Instance().GetTexture("$ENGINE/Textures/red.png"),
		AssetManager::Instance().GetTexture("$ENGINE/Textures/tempbridgge.png")
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

	m_islandDirectionNoise = new Noise();
	m_islandDirectionNoise->SetSeed(3007);
	m_islandDirectionNoise->SetNoiseScale(250.0f);
	m_islandDirectionNoise->CreateNoise();

	m_islandShapeNoise = new Noise();
	m_islandShapeNoise->SetNoiseScale(1.0f);
	m_islandShapeNoise->CreateNoise();
}

void BiomeGeneration::Uninitialise()
{
	delete m_biomeNoise;
	delete m_islandDirectionNoise;
	delete m_islandShapeNoise;
}

void BiomeGeneration::Render()
{
	EventDispatcher::SubscribeFunction<Graphics::OnEarlyRender>(std::bind(&BiomeGeneration::Draw, this));
}

void BiomeGeneration::Draw()
{
	//TILE MAP DRAWS SO THIS FUNCTION SHOULDNT EXIST

	DrawIslands();
}

unsigned int BiomeGeneration::CalculateRandomIslandIndex()
{
	srand(m_biomeNoise->GetSeed());
	return rand() % m_OccupiedIslandSpaces.size();
}

void BiomeGeneration::DrawIslands()
{
	Rectf m_initialSourceRect = Rectf(0.0f, 0.0f, -1.0f, -1.0f);;
	Rectf m_destinationRect = Rectf(0.0f, 3.0f, 1.0f, 1.0f);
	Rectf m_centre = Rectf(-10.0f, 0.0f, 1.0f, 1.0f);
	Rectf m_curIslandCentre = m_centre;
	int m_layer = 32;
	double m_rotation = 0.0;
	size_t numberOfIslands = 6;
	m_radius = 6;

	//for each island
	Rectf newDestRect = Rectf(0.0f, 0.0f, 1.0f, 1.0f);
	int iterator = 0;
	IslandSpawnDirection direction = CalculateNextIslandDirection(iterator);

	for (size_t index = 0; index < numberOfIslands; ++index)
	{
		DrawIslandCircles(newDestRect, m_initialSourceRect, m_curIslandCentre, index);

		if (index == numberOfIslands - 1)
			continue;

		//randomly pick an island centre from occupied islands
		int randomIndex = CalculateRandomIslandIndex();
		m_curIslandCentre.x = m_OccupiedIslandSpaces[randomIndex].x;
		m_curIslandCentre.y = m_OccupiedIslandSpaces[randomIndex].y;

		//find new island centre
		bool isIslandPositionEmpty = false;
		while (!isIslandPositionEmpty)
		{
			Vec2i potentialNewIslandPosition = Vec2i(0, 0);

			potentialNewIslandPosition = Vec2i((int)m_curIslandCentre.x, (int)m_curIslandCentre.y);

			switch (direction)
			{
			case IslandSpawnDirection::North:
				potentialNewIslandPosition.y = m_curIslandCentre.y + (m_radius * 2) + m_bridgeLength + 1;
				if (IsIslandSpaceFree(potentialNewIslandPosition))
				{
					DrawBridge(m_destinationRect, m_initialSourceRect, m_curIslandCentre, direction);
					m_curIslandCentre.y = potentialNewIslandPosition.y;
					isIslandPositionEmpty = true;
				}

				break;
			case IslandSpawnDirection::East:
				potentialNewIslandPosition.x = m_curIslandCentre.x + (m_radius * 2) + m_bridgeLength + 1;
				if (IsIslandSpaceFree(potentialNewIslandPosition))
				{
					DrawBridge(m_destinationRect, m_initialSourceRect, m_curIslandCentre, direction);
					m_curIslandCentre.x = potentialNewIslandPosition.x;
					isIslandPositionEmpty = true;
				}

				break;

			case IslandSpawnDirection::South:
				potentialNewIslandPosition.y = m_curIslandCentre.y - (m_radius * 2) - m_bridgeLength - 1;
				if (IsIslandSpaceFree(potentialNewIslandPosition))
				{
					DrawBridge(m_destinationRect, m_initialSourceRect, m_curIslandCentre, direction);
					m_curIslandCentre.y = potentialNewIslandPosition.y;
					isIslandPositionEmpty = true;
				}

				break;

			case IslandSpawnDirection::West:
				potentialNewIslandPosition.x = m_curIslandCentre.x - (m_radius * 2) - m_bridgeLength - 1;
				if (IsIslandSpaceFree(potentialNewIslandPosition))
				{
					DrawBridge(m_destinationRect, m_initialSourceRect, m_curIslandCentre, direction);
					m_curIslandCentre.x = potentialNewIslandPosition.x;
					isIslandPositionEmpty = true;
				}

				break;
			}
			iterator++;
			direction = CalculateNextIslandDirection(iterator);
		}
	}

	m_OccupiedIslandSpaces.clear();
}

void BiomeGeneration::DrawIslandCircles(Rectf& destRect, Rectf sourceRect, Rectf currentIslandCentre, int index)
{
	//dont need these two once tile map is used.
	int m_layer = 32;
	double m_rotation = 0.0;

	m_OccupiedIslandSpaces.push_back(Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y));

	for (int x = -m_radius; x <= m_radius; ++x)
	{
		for (int y = -m_radius; y <= m_radius; ++y)
		{
			destRect = Rectf(currentIslandCentre.x + x, currentIslandCentre.y + y, 1.0f, 1.0f);
			if (Vec2i::Dist(Vec2i((int)destRect.x, destRect.y), Vec2i(currentIslandCentre.x, currentIslandCentre.y)) <= m_radius)
			{

				//mapOfBiomesOnTileIDs[y] = 
				//GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, sm_biomeMap[RandomBiomeIndex(index)], m_layer, m_rotation, Colours::sc_white, sourceRect);
			}
			else
			{
				//int numberOfExtraTiles = CalculateIslandShape(rand() + index);
				//for (unsigned int i = 0; i < numberOfExtraTiles; ++i)
				//{
				//	int extraX = x + i;
				//	int extraY = y + i;
				//	destRect = Firelight::Maths::Rectf(currentIslandCentre.x + extraX, currentIslandCentre.y + extraY, 1.0f, 1.0f);
				//	Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, sm_biomeMap[RandomBiomeIndex(index)], m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, sourceRect);
				//}
			}
		}
	}
}

void BiomeGeneration::DrawBridge(Rectf& destRect, Rectf sourceRect, Rectf currentIslandCentre, IslandSpawnDirection direction)
{
	//dont need these two once tile map is used.
	int m_layer = 32;
	double m_rotation = 0.0;

	switch (direction)
	{
		case IslandSpawnDirection::North:
			destRect.y = currentIslandCentre.y + m_radius + 1;
			destRect.x = currentIslandCentre.x;
			break;
		case IslandSpawnDirection::East:
			destRect.x = currentIslandCentre.x + m_radius + 1;
			destRect.y = currentIslandCentre.y;
			break;

		case IslandSpawnDirection::South:
			destRect.y = currentIslandCentre.y - m_radius - 1;
			destRect.x = currentIslandCentre.x;
			break;

		case IslandSpawnDirection::West:
			destRect.x = currentIslandCentre.x - m_radius - 1;
			destRect.y = currentIslandCentre.y;
			break;
	}

	for (size_t x = 0; x < m_bridgeLength; ++x)
	{
		for (size_t y = 0; y < m_bridgeWidth; ++y)
		{
			GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, sm_biomeMap[5], m_layer, m_rotation, Colours::sc_white, sourceRect);
			switch (direction)
			{
			case IslandSpawnDirection::North:
				destRect.x = destRect.x + 1;
				break;
			case IslandSpawnDirection::East:
				destRect.y = destRect.y + 1;
				break;

			case IslandSpawnDirection::South:
				destRect.x = destRect.x - 1;
				break;

			case IslandSpawnDirection::West:
				destRect.y = destRect.y - 1;
				break;
			}
		}
		switch (direction)
		{
		case IslandSpawnDirection::North:
			destRect.y = destRect.y + 1;
			destRect.x = currentIslandCentre.x;
			break;
		case IslandSpawnDirection::East:
			destRect.x = destRect.x + 1;
			destRect.y = currentIslandCentre.y;
			break;

		case IslandSpawnDirection::South:
			destRect.y = destRect.y - 1;
			destRect.x = currentIslandCentre.x;
			break;

		case IslandSpawnDirection::West:
			destRect.x = destRect.x - 1;
			destRect.y = currentIslandCentre.y;
			break;

		}
	}
}

bool BiomeGeneration::IsIslandSpaceFree(Vec2i newIslandPosition)
{
	if (std::find(m_OccupiedIslandSpaces.begin(), m_OccupiedIslandSpaces.end(), newIslandPosition) != m_OccupiedIslandSpaces.end())
	{
		return false;
	}
	return true;
}

IslandSpawnDirection BiomeGeneration::CalculateNextIslandDirection(unsigned int noiseIndex)
{
	float* noiseData = m_islandDirectionNoise->GetNoiseData();
	float data = noiseData[noiseIndex];

	if (data >= -1.0 && data <= -0.5)
	{
		return IslandSpawnDirection::North;
	}
	if (data > -0.5 && data <= -0.0)
	{
		return IslandSpawnDirection::East;
	}
	if (data >= 0.0f && data < 0.5f)
	{
		return IslandSpawnDirection::South;
	}
	if (data >= 0.5 && data <= 1)
	{
		return IslandSpawnDirection::West;
	}
}

unsigned int BiomeGeneration::CalculateIslandShape(int perlinIndex)
{
	float* noiseData = m_islandShapeNoise->GetNoiseData();
	float data = noiseData[perlinIndex];

	if (data >= -1.0f && data <= 0.0f)
	{
		return 0;
	}
	if (data > 0.0f && data <= 0.5)
	{
		return 1;
	}
	if (data >= 0.5 && data <= 1.0f)
	{
		return 2;
	}

	return 0;
}


unsigned int BiomeGeneration::RandomBiomeIndex(unsigned int noiseIndex)
{
	float* noiseData = m_biomeNoise->GetNoiseData();
	float data = noiseData[noiseIndex];

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