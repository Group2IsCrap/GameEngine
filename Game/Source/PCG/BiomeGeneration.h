#pragma once
#include "Noise.h"
#include "Biomes.h"
#include "..\FirelightEngine\Source\Maths\Rect.h"
#include "..\FirelightEngine\Source\TileMap\TileMap.h"
#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"

#include "..\FirelightEngine\Source\TileMap\TileMap.h"
#include "BiomeInfo.h"

#include <map>

using namespace Firelight::Graphics;
using namespace Firelight::Maths;

enum class IslandSpawnDirection
{
	North,
	East,
	South,
	West
};

class BiomeGeneration
{
public:
	BiomeGeneration();

	static BiomeGeneration* Instance();
	void Initialise(Firelight::TileMap::TileMap* tileMap);
	void Uninitialise();

	unsigned int CalculateIslandShape(int perlinIndex);

	void GenerateWorld();

private:

	void DrawIslandCircles(Rectf& destRect, Rectf currentIslandCentre, int index);
	void DrawBridge(Rectf& destRect, Rectf currentIslandCentre, IslandSpawnDirection direction);

	bool IsIslandSpaceFree(Vec2i newIslandPosition);

	IslandSpawnDirection CalculateNextIslandDirection(unsigned int noiseIndex);
	unsigned int CalculateRandomIslandIndex();
	void FindNextIslandCentre(Rectf& currentIslandCentre, IslandSpawnDirection& direction, Rectf& destRect, int& iterator);
	BiomeType RandomBiomeType(unsigned int noiseIndex);

	Noise* m_biomeNoise;
	Noise* m_islandDirectionNoise;
	Noise* m_islandShapeNoise;

	Firelight::TileMap::TileMap* m_tileMap;

	static BiomeGeneration* sm_instance;
	static unsigned int mapSeed;

	std::vector< Vec2i > m_OccupiedIslandSpaces;
	std::vector< Texture*> sm_biomeMap;

	unsigned int m_biomeCount;

	unsigned int m_bridgeWidth;
	unsigned int m_bridgeLength;

	int m_radius;

};
