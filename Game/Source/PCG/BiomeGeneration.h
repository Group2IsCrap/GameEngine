#pragma once
#include "Noise.h"
#include "Biomes.h"
#include "..\FirelightEngine\Source\Maths\Rect.h"
#include "..\FirelightEngine\Source\TileMap\TileMap.h"
#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"
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
	void Initialise();
	void Uninitialise();

	unsigned int CalculateIslandShape(int perlinIndex);

	void DrawIslands();

	void Draw();
	void Render();

private:

	void DrawIslandCircles(Rectf& destRect, Rectf sourceRect, Rectf currentIslandCentre, int index);
	void DrawBridge(Rectf& destRect, Rectf sourceRect, Rectf currentIslandCentre, IslandSpawnDirection direction);

	bool IsIslandSpaceFree(Vec2i newIslandPosition);

	IslandSpawnDirection CalculateNextIslandDirection(unsigned int noiseIndex);
	unsigned int CalculateRandomIslandIndex();
	unsigned int RandomBiomeIndex(unsigned int noiseIndex);

	Noise* m_biomeNoise;
	Noise* m_islandDirectionNoise;
	Noise* m_islandShapeNoise;

	static BiomeGeneration* sm_instance;
	static unsigned int mapSeed;

	std::vector< Vec2i > m_OccupiedIslandSpaces;
	std::vector< Texture*> sm_biomeMap;
	Biome m_biome;

	unsigned int m_bridgeWidth;
	unsigned int m_bridgeLength;

	int m_radius;

};
