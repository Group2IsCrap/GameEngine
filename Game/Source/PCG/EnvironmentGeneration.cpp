#include "EnvironmentGeneration.h"
#include "../Game/Source/WorldEntities/ResourceDatabase.h"
#include "../WorldEntities/EntitySpawnerComponent.h"
#include "../Source/Maths/Random.h"

EnvironmentGeneration* EnvironmentGeneration::sm_instance = nullptr;

EnvironmentGeneration::EnvironmentGeneration()
	: m_spawnRateNoise(nullptr)
	, m_biomeInfo(nullptr)
	, m_tileMap(nullptr)
	, m_treeSpawnRate(0.05f)
	, m_rockSpawnRate(0.05f)
	, m_enemySpawnRate(0.05f)
	, m_bushSpawnRate(0.05f)
	, m_berryBushSpawnRate(0.05f)
{}

void EnvironmentGeneration::Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo)
{
	m_tileMap = tileMap;
	m_biomeInfo = biomeInfo;
	m_spawnRateNoise = new Noise();
	m_spawnRateNoise->SetSeed(Firelight::Maths::Random::RandomRange(100, 2000));
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

	int noiseTreeIndex = 0;
	int noiseRockIndex = 7;
	int noiseEnemyIndex = 12;
	int noiseBushIndex = 16;
	for (row = tiles.begin(); row != tiles.end(); row++)
	{
		for (column = row->begin(); column != row->end(); column++)
		{
			if (!(*column)->IsOccupied())
			{
				Vec3f position = Vec3f((*column)->GetDestinationRect().x, (*column)->GetDestinationRect().y, 0.0f);
				if (CanSpawnFromNoise(noiseTreeIndex, m_treeSpawnRate))
				{
					if (m_biomeInfo->biomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Forest
						|| m_biomeInfo->biomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Swamp
						|| m_biomeInfo->biomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Snow)
					{
						SpawnTree(position);
						(*column)->SetIsOccupied(true);
					}
				}
				if (CanSpawnFromNoise(noiseRockIndex, m_rockSpawnRate))
				{
					SpawnRock(position);
					(*column)->SetIsOccupied(true);
				}
				if (CanSpawnFromNoise(noiseEnemyIndex, m_enemySpawnRate))
				{
					if (m_biomeInfo->biomesOnTileIDs[(*column)->GetTileID()] == BiomeType::Swamp)
					{
						SpawnCroc(position);
						(*column)->SetIsOccupied(true);
					}
				}
				if (CanSpawnFromNoise(noiseBushIndex, m_bushSpawnRate))
				{
					SpawnBush(position);
					(*column)->SetIsOccupied(true);
				}
			}
			noiseRockIndex = noiseRockIndex + 10;
			noiseBushIndex = noiseBushIndex + 20;
			noiseEnemyIndex = noiseEnemyIndex + 30;
			noiseTreeIndex++;
		}
	}
}

void EnvironmentGeneration::SpawnTree(Vec3f position)
{
	GameEntity* treeSpawner = new GameEntity("Tree Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 0;
	spawnerComponent->respawnCooldown = 3;
	treeSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	treeSpawner->GetTransformComponent()->SetPosition(Vec3f(position.x, position.y + 5, position.z));
}

void EnvironmentGeneration::SpawnRock(Vec3f position)
{
	GameEntity* rockSpawner = new GameEntity("Rock Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 1;
	spawnerComponent->respawnCooldown = 3;
	rockSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	rockSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnCroc(Vec3f position)
{
	GameEntity* enemySpawner = new GameEntity("Crocodile Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Crocodile";
	spawnerComponent->respawnCooldown = 3;
	enemySpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	enemySpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnDeer(Vec3f position)
{
	GameEntity* enemySpawner = new GameEntity("Crocodile Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Deer";
	spawnerComponent->respawnCooldown = 3;
	enemySpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	enemySpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnBush(Vec3f position)
{
	GameEntity* bushSpawner = new GameEntity("Bush Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 2;
	spawnerComponent->respawnCooldown = 3;
	bushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	bushSpawner->GetTransformComponent()->SetPosition(position);
}

//void EnvironmentGeneration::SpawnBerryBush(Vec3f position)
//{
//
//}

bool EnvironmentGeneration::CanSpawnFromNoise(int noiseIndex, float spawnChance)
{
	if (noiseIndex < NOISE_DATA_SIZE * NOISE_DATA_SIZE)
	{
		float* noiseData = m_spawnRateNoise->GetNoiseData();
		float data = noiseData[noiseIndex];

		float compareMin = -1.0f;
		float compareMax = 1.0f;

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