#include "EnvironmentGeneration.h"
#include "Noise.h"
#include "BiomeInfo.h"

EnvironmentGeneration::EnvironmentGeneration()
	: m_spawnRateNoise(nullptr)
{}

void EnvironmentGeneration::Initialise(Firelight::TileMap::TileMap* tileMap)
{
	m_tileMap = tileMap;

	m_spawnRateNoise = new Noise();
	m_spawnRateNoise->SetSeed(3007);
	m_spawnRateNoise->SetNoiseScale(250.0f);
	m_spawnRateNoise->CreateNoise();
}

void EnvironmentGeneration::GenerateResources()
{
	// Go through the whole tile map, check the biome type if in certain biome, spawn trees, spawn rocks
	std::vector<std::vector<Firelight::TileMap::Tile*>>::iterator row;
	std::vector<Firelight::TileMap::Tile*>::iterator column;

	std::vector<std::vector<Firelight::TileMap::Tile*>> tiles = m_tileMap->GetTileMap();

	int Noiseindex = 0;
	for (row = tiles.begin(); row != tiles.end(); row++)
	{
		for (column = row->begin(); column != row->end(); column++)
		{
			if (mapOfBiomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Forest)
			{
				if ((*column)->IsOccupied())
				{
					Vec2f position = Vec2f((*column)->GetDestinationRect().x, (*column)->GetDestinationRect().y);
					if (CanSpawnTreeFromNoise(Noiseindex))
					{
						SpawnTree(position);
					}
					//else if (CanSpawnRockFromNoise(NoiseIndex))
					//{
					//	SpawnRocks();
					//}
				}
			}
			Noiseindex++;
		}
	}


}

void EnvironmentGeneration::SpawnTree(Vec2f position)
{
	//Place tree down at position
}

void EnvironmentGeneration::SpawnRocks()
{
}

bool EnvironmentGeneration::CanSpawnTreeFromNoise(int noiseIndex)
{
	float* noiseData = m_spawnRateNoise->GetNoiseData();
	float data = noiseData[noiseIndex];

	float compareMin = -1.0f;
	float compareMax = 1.0f;

	if (data >= compareMin && data <= compareMin + (compareMin/2))
	{
		return true;
	}
	if (data >= compareMax - (compareMin / 2) && data < compareMax)
	{
		return false;
	}
	return false;
}

bool EnvironmentGeneration::CanSpawnRockFromNoise(int noiseIndex)
{
	float* noiseData = m_spawnRateNoise->GetNoiseData();
	float data = noiseData[noiseIndex];

	float compareMin = -1.0f;
	float compareMax = 1.0f;

	if (data >= compareMin && data <= compareMin + (compareMin / 2))
	{
		return true;
	}
	if (data >= compareMax - (compareMin / 2) && data < compareMax)
	{
		return false;
	}
	return false;
}