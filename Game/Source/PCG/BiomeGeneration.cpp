#include "BiomeGeneration.h"

#include "../FirelightEngine/Source/Graphics/GraphicsHandler.h"
#include "../FirelightEngine/Source/Graphics/AssetManager.h"
#include "../FirelightEngine/Source/Graphics/SpriteBatch.h"
#include <../FirelightEngine/Source/Events/EventDispatcher.h>
#include <../FirelightEngine/Source/Maths/Random.h>
#include "PCGEvents.h"

using namespace Firelight::Events;

unsigned int BiomeGeneration::sm_mapSeed = 1234;

BiomeGeneration::BiomeGeneration()
	: m_biomeNoise(nullptr)
	, m_islandDirectionNoise(nullptr)
	, m_islandShapeNoise(nullptr)
	, m_biomeInfo(nullptr)
	, m_tileMap(nullptr)
	, m_walkableBoxZones()
	, m_islandCentres()
	, m_OccupiedIslandSpaces()
	, m_initialCentre(Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, m_biomeCount((unsigned int)BiomeType::Bridge)
	, m_bridgeWidth(2)
	, m_bridgeLength(3)
	, m_islandRadii(15)
	, m_seed(0)
	, m_numberOfIslands(6)
	, m_playerWasOnBridge(false)
	, testPosition(Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 1.0f))
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

void BiomeGeneration::Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo)
{
	m_seed = Firelight::Maths::Random::RandomRange(100, 2000);

	m_tileMap = tileMap;
	m_biomeInfo = biomeInfo;

	sm_biomeTextures = { AssetManager::Instance().GetTexture("Sprites/Terrain/water.png"),
					   AssetManager::Instance().GetTexture("Sprites/Terrain/grass.png"),
					   AssetManager::Instance().GetTexture("Sprites/Terrain/swamp.png"),
					   AssetManager::Instance().GetTexture("Sprites/Terrain/snow.png"),
					   AssetManager::Instance().GetTexture("$ENGINE/Textures/tempbridgge.png"),
					   AssetManager::Instance().GetTexture("Sprites/Terrain/bridge.png") };

	m_biomeNoise = new Noise();
	m_biomeNoise->SetSeed(m_seed);
	m_biomeNoise->SetNoiseScale(250.0f);
	m_biomeNoise->CreateNoise();

	m_islandDirectionNoise = new Noise();
	m_islandDirectionNoise->SetSeed(m_seed);
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
	Rectf destinationRect = Rectf(10.0f, 10.0f, 1.0f, 1.0f);
	Rectf curIslandCentre = m_initialCentre;

	//for each island
	Rectf newDestRect = Rectf(0.0f, 0.0f, 1.0f, 1.0f);
	int iterator = 0;
	DrawFirstIsland(newDestRect, curIslandCentre, 0);
	m_islandCentres.emplace_back(curIslandCentre);
	IslandSpawnDirection direction = CalculateNextIslandDirection(iterator);
	int randomIndex = CalculateRandomIslandIndex();
	curIslandCentre.x = (float)m_OccupiedIslandSpaces[randomIndex].x;
	curIslandCentre.y = (float)m_OccupiedIslandSpaces[randomIndex].y;
	FindNextIslandCentre(curIslandCentre, direction, destinationRect, iterator);

	for (size_t index = 0; index < m_numberOfIslands; ++index)
	{
		m_islandCentres.emplace_back(curIslandCentre);
		DrawIslandCircles(newDestRect, curIslandCentre, (int)index);

		if (index == m_numberOfIslands - 1)
			continue;

		//randomly pick an island centre from occupied islands
		int randomIndex = CalculateRandomIslandIndex();
		curIslandCentre.x = (float)m_OccupiedIslandSpaces[randomIndex].x;
		curIslandCentre.y = (float)m_OccupiedIslandSpaces[randomIndex].y;

		FindNextIslandCentre(curIslandCentre, direction, destinationRect, iterator);
	}

	m_OccupiedIslandSpaces.clear();
}

void BiomeGeneration::DrawFirstIsland(Rectf& destRect, const Rectf currentIslandCentre, int index)
{
	m_OccupiedIslandSpaces.push_back(Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y));

	for (int x = -m_islandRadii; x <= m_islandRadii; ++x)
	{
		for (int y = -m_islandRadii; y <= m_islandRadii; ++y)
		{
			destRect = Rectf(currentIslandCentre.x + x, currentIslandCentre.y + y, 1.0f, 1.0f);
			if (Vec2i::Dist(Vec2i((int)destRect.x, (int)destRect.y), Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y)) <= m_islandRadii)
			{
				Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
				if (tile != nullptr)
				{
					m_biomeInfo->mapOfBiomesOnTileIDs[tile->GetTileID()] = BiomeType::Forest;
					tile->SetTileTexture(sm_biomeTextures[(int)BiomeType::Forest]);
					tile->SetIsDrawn(true);
				}
			}
			else
			{
				// This is adding variety to the island outlines
				unsigned int numberOfExtraTiles = CalculateIslandShape(rand() + index);
				for (unsigned int i = 0; i < numberOfExtraTiles; ++i)
				{
					int extraX = x + i;
					int extraY = y + i;
					destRect = Firelight::Maths::Rectf(currentIslandCentre.x + extraX, currentIslandCentre.y + extraY, 1.0f, 1.0f);
					Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
					if (tile != nullptr)
					{
						m_biomeInfo->mapOfBiomesOnTileIDs[tile->GetTileID()] = BiomeType::Forest;
						tile->SetTileTexture(sm_biomeTextures[(int)BiomeType::Forest]);
						tile->SetIsDrawn(true);
					}
				}
			}
		}
	}
}

void BiomeGeneration::DrawIslandCircles(Rectf& destRect, const Rectf currentIslandCentre, int index)
{
	m_OccupiedIslandSpaces.push_back(Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y));

	for (int x = -m_islandRadii; x <= m_islandRadii; ++x)
	{
		for (int y = -m_islandRadii; y <= m_islandRadii; ++y)
		{
			destRect = Rectf(currentIslandCentre.x + x, currentIslandCentre.y + y, 1.0f, 1.0f);
			if (Vec2i::Dist(Vec2i((int)destRect.x, (int)destRect.y), Vec2i((int)currentIslandCentre.x, (int)currentIslandCentre.y)) <= m_islandRadii)
			{
				Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
				if (tile != nullptr)
				{
					m_biomeInfo->mapOfBiomesOnTileIDs[tile->GetTileID()] = RandomBiomeType(index);
					tile->SetTileTexture(sm_biomeTextures[(int)RandomBiomeType(index)]);
					tile->SetIsDrawn(true);
				}
			}
			else
			{
				// This is adding variety to the island outlines
				unsigned int numberOfExtraTiles = CalculateIslandShape(rand() + index);
				for (unsigned int i = 0; i < numberOfExtraTiles; ++i)
				{
					int extraX = x + i;
					int extraY = y + i;
					destRect = Firelight::Maths::Rectf(currentIslandCentre.x + extraX, currentIslandCentre.y + extraY, 1.0f, 1.0f);
					Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
					if (tile != nullptr)
					{
						m_biomeInfo->mapOfBiomesOnTileIDs[tile->GetTileID()] = RandomBiomeType(index);
						tile->SetTileTexture(sm_biomeTextures[(int)RandomBiomeType(index)]);
						tile->SetIsDrawn(true);
					}
				}
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
			potentialNewIslandPosition.y = (int)(currentIslandCentre.y + (m_islandRadii * 2) + m_bridgeLength + 1);
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.y = (float)potentialNewIslandPosition.y;
				isIslandPositionEmpty = true;
			}

			break;
		case IslandSpawnDirection::East:
			potentialNewIslandPosition.x = (int)(currentIslandCentre.x + (m_islandRadii * 2) + m_bridgeLength + 1);
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.x = (float)potentialNewIslandPosition.x;
				isIslandPositionEmpty = true;
			}

			break;

		case IslandSpawnDirection::South:
			potentialNewIslandPosition.y = (int)(currentIslandCentre.y - (m_islandRadii * 2) - m_bridgeLength - 1);
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.y = (float)potentialNewIslandPosition.y;
				isIslandPositionEmpty = true;
			}

			break;

		case IslandSpawnDirection::West:
			potentialNewIslandPosition.x = (int)(currentIslandCentre.x - (m_islandRadii * 2) - m_bridgeLength - 1);
			if (IsIslandSpaceFree(potentialNewIslandPosition))
			{
				DrawBridge(destRect, currentIslandCentre, direction);
				currentIslandCentre.x = (float)potentialNewIslandPosition.x;
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
	switch (direction)
	{
		case IslandSpawnDirection::North:
			destRect.y = currentIslandCentre.y + m_islandRadii + 1;
			destRect.x = currentIslandCentre.x;
			break;
		case IslandSpawnDirection::East:
			destRect.x = currentIslandCentre.x + m_islandRadii + 1;
			destRect.y = currentIslandCentre.y;
			break;

		case IslandSpawnDirection::South:
			destRect.y = currentIslandCentre.y - m_islandRadii - 1;
			destRect.x = currentIslandCentre.x;
			break;

		case IslandSpawnDirection::West:
			destRect.x = currentIslandCentre.x - m_islandRadii - 1;
			destRect.y = currentIslandCentre.y;
			break;
	}

	Firelight::Maths::Vec2f highestPos = Firelight::Maths::Vec2f(destRect.x, destRect.y);
	Firelight::Maths::Vec2f lowestPos = Firelight::Maths::Vec2f(destRect.x, destRect.y);
	for (size_t x = 0; x < m_bridgeLength; ++x)
	{
		for (size_t y = 0; y < m_bridgeWidth; ++y)
		{
			OutputLowestAndHighestVec(lowestPos, highestPos, destRect);
			Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(destRect.x, destRect.y));
			m_biomeInfo->mapOfBiomesOnTileIDs[tile->GetTileID()] = BiomeType::Bridge;
			tile->SetTileTexture(sm_biomeTextures[(int)BiomeType::Bridge + 1]);
			tile->SetIsDrawn(true);

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

	m_walkableBoxZones.emplace_back(Firelight::Maths::Rectf(lowestPos.x, lowestPos.y, std::abs(lowestPos.x - highestPos.x), std::abs(lowestPos.y - highestPos.y)));
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
	if (data > -0.5 && data <= 0.0)
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
	return IslandSpawnDirection::North;
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

bool BiomeGeneration::IsInVoid(Rectf position)
{
	Vec2f tileMapBot = m_tileMap->GetBottomLeftTilePos();
	Vec2f tileMapTop = m_tileMap->GetTopRightTilePos();


	if (!IsPositionBetweenTwoPoints(position, tileMapBot, tileMapTop))
	{
		return true;
	}

	for (auto box : m_walkableBoxZones)
	{
		Vec2f position1 = Vec2f(box.x, box.y);
		Vec2f position2 = Vec2f(box.x + box.w + 1, box.y + box.h + 1);
		if (IsPositionBetweenTwoPoints(position, position1, position2))
		{
			return false;
		}
	}

	for (auto centre : m_islandCentres)
	{
		if (Vec2i::Dist(Vec2i((int)position.x, (int)position.y), Vec2i((int)centre.x, (int)centre.y)) <= m_islandRadii)
		{
			return false;
		}
	}
	return true;
}

void BiomeGeneration::CheckCurrentPlayerBiomeType(Rectf playerPosition)
{
	for (auto box : m_walkableBoxZones)
	{
		Vec2f position1 = Vec2f(box.x - 1, box.y - 1);
		Vec2f position2 = Vec2f(box.x + box.w + 1, box.y + box.h + 1);
		if (IsPositionBetweenTwoPoints(Rectf(playerPosition.x, playerPosition.y - 1, playerPosition.w, playerPosition.h), position1, position2))
		{
			m_playerWasOnBridge = true;
		}
		else if (m_playerWasOnBridge)
		{
			m_playerWasOnBridge = false;
			Firelight::TileMap::Tile* tile = m_tileMap->GetTileAtPosition(Vec2f(playerPosition.x, playerPosition.y - 2));
			unsigned int tileID = tile->GetTileID();
			BiomeMusicData biomeMusicData;
			biomeMusicData.biome = m_biomeInfo->mapOfBiomesOnTileIDs[tileID];
			biomeMusicData.playerPosition = Vector3D(playerPosition.x, playerPosition.y, 0.0f);
			Firelight::Events::EventDispatcher::InvokeListeners<PCGEvents::OnPlayerCrossBridge>((void*)&biomeMusicData);
		}
	}
}

void BiomeGeneration::OutputLowestAndHighestVec(Vec2f& lowestPos, Vec2f& highestPos, Rectf rectVal)
{
	if (highestPos.y < rectVal.y)
	{
		highestPos.y = rectVal.y;
	}
	if (highestPos.x < rectVal.x)
	{
		highestPos.x = rectVal.x;
	}

	if (lowestPos.y > rectVal.y)
	{
		lowestPos.y = rectVal.y;
	}
	if (lowestPos.x > rectVal.x)
	{
		lowestPos.x = rectVal.x;
	}
}

bool BiomeGeneration::IsPositionBetweenTwoPoints(Rectf position, Vec2f point1, Vec2f point2)
{
	if ((position.x >= point1.x) &&
		(position.x <= point2.x) &&
		(position.y >= point1.y) &&
		(position.y <= point2.y))
	{
		return true;
	}
	return false;
}