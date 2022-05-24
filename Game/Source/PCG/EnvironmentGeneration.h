#pragma once

#include "../FirelightEngine/Source/TileMap/TileMap.h"
#include "BiomeInfo.h"
#include "Noise.h"

using namespace Firelight::Maths;

class EnvironmentGeneration
{
public:

	static EnvironmentGeneration* Instance();
	EnvironmentGeneration();
	void GenerateResources();
	void Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo);

private:
	void SpawnTree(Vec3f position);
	void SpawnRock(Vec3f position);

	bool CanSpawnTreeFromNoise(int noiseIndex);
	bool CanSpawnRockFromNoise(int noiseIndex);

	static EnvironmentGeneration* sm_instance;

	Noise* m_spawnRateNoise;
	BiomeInfo* m_biomeInfo;
	Firelight::TileMap::TileMap* m_tileMap;
};

