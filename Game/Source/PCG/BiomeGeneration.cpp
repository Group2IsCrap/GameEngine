#include "BiomeGeneration.h"
#include "../FirelightEngine/Source/Graphics/GraphicsHandler.h"
#include "../FirelightEngine/Source/Graphics/Data/Texture.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"
#include "../FirelightEngine/Source/Maths/Rect.h"
#include "../FirelightEngine/Source/Graphics/GraphicsEvents.h"
#include "../FirelightEngine/Source/Graphics/SpriteBatch.h"
#include "../FirelightEngine/Source/Events/EventDispatcher.h"
#include "../FirelightEngine/Source/ECS/Systems/TilemapSystem.h"

using namespace Firelight::Events;

unsigned int BiomeGeneration::mapSeed = 1234;

BiomeGeneration::BiomeGeneration()
	: m_biomeNoise(nullptr)
	, m_islandDirectionNoise(nullptr)
	, m_islandShapeNoise(nullptr)
	, m_tileMap(nullptr)
	, m_biomeCount((unsigned int)BiomeType::Bridge)
	, m_bridgeWidth(2)
	, m_bridgeLength(3)
	, m_radius(3)
{}

BiomeGeneration* BiomeGeneration::sm_instance = nullptr;

BiomeGeneration* BiomeGeneration::Instance()
{
	if (sm_instance == nullptr)
	{
		sm_instance = new BiomeGeneration();
	}
	return sm_instance;
}

void BiomeGeneration::Initialise(Firelight::TileMap::TileMap* tileMap)
{
	m_tileMap = tileMap;

	sbiomeTextures = { AssetManager::Instance().GetTexture("$ENGINE/Textures/green.png"),
					    AssetManager::Instance().GetTexture("$ENGINE/Textures/brown.png"),
						AssetManager::Instance().GetTexture("$ENGINE/Textures/white.png"),
						AssetManager::Instance().GetTexture("$ENGINE/Textures/tempbridgge.png") };

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

unsigned int BiomeGeneration::CalculateRandomIslandIndex()
{
	srand(m_biomeNoise->GetSeed());
	return rand() % m_OccupiedIslandSpaces.size();
}

void BiomeGeneration::GenerateWorld()
{
	Rectf m_destinationRect = Rectf(10.0f, 10.0f, 1.0f, 1.0f);
	Rectf m_centre = Rectf(10.0f, 10.0f, 1.0f, 1.0f);
	Rectf m_curIslandCentre = m_centre;
	size_t numberOfIslands = 6;
	m_radius = 6;

	//for each island
	Rectf newDestRect = Rectf(0.0f, 0.0f, 1.0f, 1.0f);
	int iterator = 0;
	IslandSpawnDirection direction = CalculateNextIslandDirection(iterator);

	for (size_t index = 0; index < numberOfIslands; ++index)
	{
		DrawIslandCircles(newDestRect, m_curIslandCentre, index);

		if (index == numberOfIslands - 1)
			continue;

		//randomly pick an island centre from occupied islands
		int randomIndex = CalculateRandomIslandIndex();
		m_curIslandCentre.x = m_OccupiedIslandSpaces[randomIndex].x;
		m_curIslandCentre.y = m_OccupiedIslandSpaces[randomIndex].y;

		FindNextIslandCentre(m_curIslandCentre, direction, m_destinationRect, iterator);
	}

	m_OccupiedIslandSpaces.clear();
}

void BiomeGeneration::DrawIslandCircles(Rectf& destRect, Rectf currentIslandCentre, int index)
{
	m_OccupiedIslandSpaces.push_back(Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y));

	for (int x = -m_radius; x <= m_radius; ++x)
	{
		for (int y = -m_radius; y <= m_radius; ++y)
		{
			destRect = Rectf(currentIslandCentre.x + x, currentIslandCentre.y + y, 1.0f, 1.0f);
			if (Vec2i::Dist(Vec2i((int)destRect.x, destRect.y), Vec2i(currentIslandCentre.x, currentIslandCentre.y)) <= m_radius)
			{
				Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
				if (tile != nullptr)
				{
					mapOfBiomesOnTileIDs[tile->GetTileID()] = RandomBiomeType(index);
					tile->SetTileTexture(sbiomeTextures[(int)RandomBiomeType(index)]);
					tile->test = true;
				}
			}
			else
			{
				// This is adding variety to the island shapes
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

void BiomeGeneration::FindNextIslandCentre(Rectf& currentIslandCentre, IslandSpawnDirection& direction, Rectf& destRect, int& iterator)
{
	bool isIslandPositionEmpty = false;
	while (!isIslandPositionEmpty)
	{
		Vec2i potentialNewIslandPosition = Vec2i(0, 0);

		potentialNewIslandPosition = Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y);

		switch (direction)
		{
		case IslandSpawnDirection::North:
			potentialNewIslandPosition.y = currentIslandCentre.y + (m_radius * 2) + m_bridgeLength + 1;
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.y = potentialNewIslandPosition.y;
				isIslandPositionEmpty = true;
			}

			break;
		case IslandSpawnDirection::East:
			potentialNewIslandPosition.x = currentIslandCentre.x + (m_radius * 2) + m_bridgeLength + 1;
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.x = potentialNewIslandPosition.x;
				isIslandPositionEmpty = true;
			}

			break;

		case IslandSpawnDirection::South:
			potentialNewIslandPosition.y = currentIslandCentre.y - (m_radius * 2) - m_bridgeLength - 1;
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.y = potentialNewIslandPosition.y;
				isIslandPositionEmpty = true;
			}

			break;

		case IslandSpawnDirection::West:
			potentialNewIslandPosition.x = currentIslandCentre.x - (m_radius * 2) - m_bridgeLength - 1;
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.x = potentialNewIslandPosition.x;
				isIslandPositionEmpty = true;
			}

			break;
		}
		iterator++;
		direction = CalculateNextIslandDirection(iterator);
	}
}

void BiomeGeneration::DrawBridge(Rectf& destRect, Rectf currentIslandCentre, IslandSpawnDirection direction)
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
			Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
			mapOfBiomesOnTileIDs[tile->GetTileID()] = BiomeType::Bridge;
			tile->SetTileTexture(sbiomeTextures[(int)BiomeType::Bridge]);

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


BiomeType BiomeGeneration::RandomBiomeType(unsigned int noiseIndex)
{
	float* noiseData = m_biomeNoise->GetNoiseData();
	float data = noiseData[noiseIndex];

	float compareMin = -1.0f;
	float compareMax = 1.0f;

	float comparisonSampleSize = compareMax - compareMin;
	float adjustmentSize = comparisonSampleSize / m_biomeCount;


	if (data >= compareMin && data <= compareMin + adjustmentSize)
	{
		return BiomeType::Forest;
	}
	if (data > compareMin + adjustmentSize && data <= compareMax - adjustmentSize)
	{
		return BiomeType::Swamp;
	}
	if (data >= compareMax - adjustmentSize && data < compareMax)
	{
		return BiomeType::Snow;
	}
	return BiomeType::Forest;
}