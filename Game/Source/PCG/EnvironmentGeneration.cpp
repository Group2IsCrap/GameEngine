#include "EnvironmentGeneration.h"
#include "../Game/Source/WorldEntities/ResourceDatabase.h"
#include "../WorldEntities/EntitySpawnerComponent.h"
#include "../Source/Maths/Random.h"

EnvironmentGeneration* EnvironmentGeneration::sm_instance = nullptr;

EnvironmentGeneration::EnvironmentGeneration()
	: m_spawnRateNoise(nullptr)
	, m_biomeInfo(nullptr)
	, m_tileMap(nullptr)
	, m_treeSpawnRate(0.1f)
	, m_rockSpawnRate(0.05f)
	, m_enemySpawnRate(0.009f)
	, m_passiveAISpawnRate(0.04f)
	, m_snowAISpawnRate(0.04)
	, m_snowEnemySpawnRate(0.01)
	, m_bushSpawnRate(0.1f)
	, m_berryBushSpawnRate(0.05f)
	, m_respawnCooldown(20.0)
	, m_noiseIndices({0, 3, 6, 9})
{}

void EnvironmentGeneration::Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo)
{
	m_tileMap = tileMap;
	m_biomeInfo = biomeInfo;
	m_spawnRateNoise = new Noise();
	m_spawnRateNoise->SetSeed(Firelight::Maths::Random::RandomRange(100, 2000));
	m_spawnRateNoise->SetNoiseScale(100.0f);
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

	for (row = tiles.begin(); row != tiles.end(); row++)
	{
		for (column = row->begin(); column != row->end(); column++)
		{
			if (!(*column)->IsOccupied())
			{
				Vec3f position = Vec3f((*column)->GetDestinationRect().x, (*column)->GetDestinationRect().y, 0.0f);

				SpawnResourcesForAllBiomes((*column), position);
				SpawnForestResources((*column), position);
				SpawnSwampResources((*column), position);
				SpawnSnowResources((*column), position);

				for (size_t index = 0; index < m_noiseIndices.size(); ++index)
				{
					m_noiseIndices[index] += 1;
				}
			}
		}
	}
}

void EnvironmentGeneration::SpawnTree(Vec3f position)
{
	GameEntity* treeSpawner = new GameEntity("Tree Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 0;
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	treeSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	treeSpawner->GetTransformComponent()->SetPosition(Vec3f(position.x, position.y + 5, position.z));
}

void EnvironmentGeneration::SpawnRock(Vec3f position)
{
	GameEntity* rockSpawner = new GameEntity("Rock Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 1;
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	rockSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	rockSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnOre(Vec3f position)
{
	GameEntity* oreSpawner = new GameEntity("Ore Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 4;
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	oreSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	oreSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnCroc(Vec3f position)
{
	GameEntity* enemySpawner = new GameEntity("Crocodile Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Crocodile";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	enemySpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	enemySpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnDeer(Vec3f position)
{
	GameEntity* enemySpawner = new GameEntity("Deer Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Deer";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
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

void EnvironmentGeneration::SpawnBerryBush(Vec3f position)
{
	GameEntity* berryBushSpawner = new GameEntity("Berry Bush Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 3;
	spawnerComponent->respawnCooldown = 3;
	berryBushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	berryBushSpawner->GetTransformComponent()->SetPosition(position);
}


void EnvironmentGeneration::SpawnBear(Vec3f position)
{
	GameEntity* bushSpawner = new GameEntity("Bear Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Bear";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	bushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	bushSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnBunny(Vec3f position)
{
	GameEntity* bushSpawner = new GameEntity("Bunny Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Bunny";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	bushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	bushSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnSnowBear(Vec3f position)
{
	GameEntity* bushSpawner = new GameEntity("Snow Bear Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "SnowBear";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	bushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	bushSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnSnowBunny(Vec3f position)
{
	GameEntity* bushSpawner = new GameEntity("Snow Bunny Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "SnowBunny";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	bushSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	bushSpawner->GetTransformComponent()->SetPosition(position);
}

void EnvironmentGeneration::SpawnSlime(Vec3f position)
{
	GameEntity* SlimeSpawner = new GameEntity("Slime Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Slime";
	spawnerComponent->respawnCooldown = m_respawnCooldown;
	SlimeSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
	SlimeSpawner->GetTransformComponent()->SetPosition(position);
}

//void EnvironmentGeneration::SpawnBerryBush(Vec3f position)
//{
//
//}

void EnvironmentGeneration::SpawnResourcesForAllBiomes(Firelight::TileMap::Tile* tile, Vec3f position)
{
	if (m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Forest
		|| m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Swamp
		|| m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Snow)
	{
		int randomVal = Firelight::Maths::Random::RandomRange(0, 2);
		int randomVal2 = Firelight::Maths::Random::RandomRange(0, 1);
		
		switch (randomVal)
		{
		case 0:
			if (CanSpawnFromNoise(m_noiseIndices[0], m_treeSpawnRate))
			{
				SpawnTree(position);
				tile->SetIsOccupied(true);
			}
		break;
		case 1:
			if (CanSpawnFromNoise(m_noiseIndices[1], m_rockSpawnRate))
			{
				if (randomVal2 == 0)
				{
					SpawnRock(position);
				}
				else
				{
					SpawnOre(position);
				}
				tile->SetIsOccupied(true);
			}
		break;
		case 2:
			if (CanSpawnFromNoise(m_noiseIndices[2], m_bushSpawnRate))
			{
				if (randomVal2 == 0)
				{
					SpawnBush(position);
				}
				else
				{
					SpawnBerryBush(position);
				}
				tile->SetIsOccupied(true);
			}
		break;
		}
	}
}

void EnvironmentGeneration::SpawnForestResources(Firelight::TileMap::Tile* tile, Vec3f position)
{
	if (m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Forest)
	{
		int randomVal = Firelight::Maths::Random::RandomRange(0, 3);

		switch (randomVal)
		{
		case 0:
			if (CanSpawnFromNoise(m_noiseIndices[0], m_enemySpawnRate))
			{
				SpawnBear(position);
				tile->SetIsOccupied(true);
			}
			break;
		case 1:
			if (CanSpawnFromNoise(m_noiseIndices[1], m_passiveAISpawnRate))
			{
				SpawnBunny(position);
				tile->SetIsOccupied(true);
			}
			break;
		case 2:
			if (CanSpawnFromNoise(m_noiseIndices[2], m_enemySpawnRate))
			{
				SpawnSlime(position);
				tile->SetIsOccupied(true);
			}
			break;
		case 3:
			if (CanSpawnFromNoise(m_noiseIndices[2], m_passiveAISpawnRate))
			{
				SpawnDeer(position);
				tile->SetIsOccupied(true);
			}
			break;
		}
	}
}
void EnvironmentGeneration::SpawnSwampResources(Firelight::TileMap::Tile* tile, Vec3f position)
{
	if (m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Swamp)
	{
		if (CanSpawnFromNoise(m_noiseIndices[2], m_enemySpawnRate))
		{
			SpawnCroc(position);
			tile->SetIsOccupied(true);
		}
	}
}
void EnvironmentGeneration::SpawnSnowResources(Firelight::TileMap::Tile* tile, Vec3f position)
{
	if (m_biomeInfo->biomesOnTileIDs[tile->GetTileID()] == BiomeType::Snow)
	{
		int randomVal = Firelight::Maths::Random::RandomRange(0, 1);

		switch (randomVal)
		{
		case 0:
			if (CanSpawnFromNoise(m_noiseIndices[0], m_snowEnemySpawnRate))
			{
				SpawnSnowBear(position);
				tile->SetIsOccupied(true);
			}
			break;
		case 1:
			if (CanSpawnFromNoise(m_noiseIndices[1], m_snowAISpawnRate))
			{
				SpawnSnowBunny(position);
				tile->SetIsOccupied(true);
			}
			break;
		}
	}
}


bool EnvironmentGeneration::CanSpawnFromNoise(int noiseIndex, float spawnChance)
{
	if (noiseIndex < NOISE_DATA_SIZE * NOISE_DATA_SIZE)
	{
		int randomVal = Random::InRange(0, 1);

		float* noiseData = m_spawnRateNoise->GetNoiseData();
		float data = noiseData[noiseIndex];

		float compareMin = -1.0f;
		float compareMax = 1.0f;

		if (data >= compareMin && data <= compareMin + spawnChance)
		{
			return true;
		}
		if (data >= compareMin + spawnChance && data < compareMax)
		{
			return false;
		}
	}
	return false;
}