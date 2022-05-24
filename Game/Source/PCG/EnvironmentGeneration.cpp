#include "EnvironmentGeneration.h"
#include "BiomeInfo.h"
#include "../Game/Source/WorldEntities/ResourceDatabase.h"

EnvironmentGeneration* EnvironmentGeneration::sm_instance = nullptr;

EnvironmentGeneration::EnvironmentGeneration()
	: m_spawnRateNoise(nullptr)
	, m_biomeInfo(nullptr)
	, m_tileMap(nullptr)
{}

void EnvironmentGeneration::Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo)
{
	m_tileMap = tileMap;
	m_biomeInfo = biomeInfo;
	m_spawnRateNoise = new Noise();
	m_spawnRateNoise->SetSeed(3007);
	m_spawnRateNoise->SetNoiseScale(250.0f);
	m_spawnRateNoise->CreateNoise();
}

EnvironmentGeneration* EnvironmentGeneration::Instance()
{
	if (sm_instance == nullptr)
	{
		sm_instance = new EnvironmentGeneration();
	}
	return sm_instance;
}

void EnvironmentGeneration::GenerateResources()
{
	// Go through the whole tile map, check the biome type if in certain biome, spawn trees, spawn rocks
	std::vector<std::vector<Firelight::TileMap::Tile*>>::iterator row;
	std::vector<Firelight::TileMap::Tile*>::iterator column;

	std::vector<std::vector<Firelight::TileMap::Tile*>> tiles = m_tileMap->GetTileMap();

	int noiseTreeindex = 0;
	int noiseRockindex = 10;
	for (row = tiles.begin(); row != tiles.end(); row++)
	{
		for (column = row->begin(); column != row->end(); column++)
		{
			if (m_biomeInfo->mapOfBiomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Forest)
			{
				if (!(*column)->IsOccupied())
				{
					Vec3f position = Vec3f((*column)->GetDestinationRect().x, (*column)->GetDestinationRect().y, 0.0f);
					if (CanSpawnTreeFromNoise(noiseTreeindex))
					{
						SpawnTree(position);
						(*column)->SetIsOccupied(true);
					}
					else if (CanSpawnRockFromNoise(noiseRockindex))
					{
						SpawnRock(position);
						(*column)->SetIsOccupied(true);
					}
				}
				noiseRockindex = noiseRockindex + 10;
				noiseTreeindex++;
			}
		}
	}
}

void EnvironmentGeneration::SpawnTree(Vec3f position)
{
	ResourceEntity* treeEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(0);
	treeEntity->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnRock(Vec3f position)
{
	ResourceEntity* rockEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(1);
	rockEntity->GetTransformComponent()->SetPosition(position);
}

bool EnvironmentGeneration::CanSpawnTreeFromNoise(int noiseIndex)
{
	if (noiseIndex > NOISE_DATA_SIZE * NOISE_DATA_SIZE)
	{
		float* noiseData = m_spawnRateNoise->GetNoiseData();
		float data = noiseData[noiseIndex];

		float compareMin = -1.0f;
		float compareMax = 1.0f;

		float spawnChance = 0.05f;

		if (data >= compareMin && data <= compareMin + spawnChance)
		{
			return true;
		}
		if (data >= compareMin - spawnChance && data < compareMax)
		{
			return false;
		}
	}
	return false;
}

bool EnvironmentGeneration::CanSpawnRockFromNoise(int noiseIndex)
{
	if (noiseIndex > NOISE_DATA_SIZE * NOISE_DATA_SIZE)
	{
		float* noiseData = m_spawnRateNoise->GetNoiseData();
		float data = noiseData[noiseIndex];

		float compareMin = -1.0f;
		float compareMax = 1.0f;

		float spawnChance = 0.05f;
		if (data >= compareMin && data <= compareMin + spawnChance)
		{
			return true;
		}
		if (data >= compareMin - spawnChance && data < compareMax)
		{
			return false;
		}
	}
	return false;
}