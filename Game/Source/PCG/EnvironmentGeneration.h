#pragma once

#include "../FirelightEngine/Source/TileMap/TileMap.h"
#include "Noise.h"

using namespace Firelight::Maths;

class EnvironmentGeneration
{
public:

	EnvironmentGeneration();
	void GenerateResources();
	void Initialise(Firelight::TileMap::TileMap* tilemap);

private:
	void SpawnTree(Vec2f position);
	void SpawnRocks();

	bool CanSpawnTreeFromNoise(int noiseIndex);
	bool CanSpawnRockFromNoise(int noiseIndex);

	Noise* m_spawnRateNoise;

	Firelight::TileMap::TileMap* m_tileMap;
};

