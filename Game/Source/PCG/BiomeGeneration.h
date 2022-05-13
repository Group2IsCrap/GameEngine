#pragma once
#include "Noise.h"
#include "Biomes.h"
#include "..\FirelightEngine\Source\Maths\Rect.h"
#include <map>

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

	void DrawIslandCircles(Firelight::Maths::Rectf& destRect, Firelight::Maths::Rectf sourceRect, Firelight::Maths::Rectf currentIslandCentre, int index);
	void DrawBridge(Firelight::Maths::Rectf& destRect, Firelight::Maths::Rectf sourceRect, Firelight::Maths::Rectf currentIslandCentre, IslandSpawnDirection direction);

	bool IsIslandSpaceFree(Firelight::Maths::Vec2i newIslandPosition);

	IslandSpawnDirection CalculateNextIslandDirection(unsigned int noiseIndex);
	unsigned int CalculateRandomIslandIndex();
	unsigned int RandomBiomeIndex(unsigned int noiseIndex);

	Noise* m_biomeNoise;
	Noise* m_islandDirectionNoise;
	Noise* m_islandShapeNoise;

	static BiomeGeneration* sm_instance;
	static unsigned int mapSeed;

	std::vector< Firelight::Maths::Vec2i > m_OccupiedIslandSpaces;
	std::vector< Firelight::Graphics::Texture*> sm_biomeMap;
	Biome m_biome;

	unsigned int m_bridgeWidth;
	unsigned int m_bridgeLength;

	int m_radius;

};
