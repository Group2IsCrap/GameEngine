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
	void SpawnBush(Vec3f position);
	//void SpawnBerryBush(Vec3f position);

	void SpawnCroc(Vec3f position);
	void SpawnDeer(Vec3f position);
	void SpawnBear(Vec3f position);
	void SpawnBunny(Vec3f position);
	void SpawnSnowBear(Vec3f position);
	void SpawnSnowBunny(Vec3f position);
	void SpawnSlime(Vec3f position);

	void SpawnResourcesForAllBiomes(Firelight::TileMap::Tile* tile, Vec3f position);
	void SpawnForestResources(Firelight::TileMap::Tile* tile, Vec3f position);
	void SpawnSwampResources(Firelight::TileMap::Tile* tile, Vec3f position);
	void SpawnSnowResources(Firelight::TileMap::Tile* tile, Vec3f position);

	bool CanSpawnFromNoise(int noiseIndex, float spawnChance);

	static EnvironmentGeneration* sm_instance;

	Noise* m_spawnRateNoise;
	BiomeInfo* m_biomeInfo;
	Firelight::TileMap::TileMap* m_tileMap;

	float m_treeSpawnRate;
	float m_rockSpawnRate;
	float m_enemySpawnRate;
	float m_passiveAISpawnRate;
	float m_snowAISpawnRate;
	float m_snowEnemySpawnRate;
	float m_bushSpawnRate;
	float m_berryBushSpawnRate;

	double m_respawnCooldown;

	std::vector<unsigned int> m_noiseIndices;
};

