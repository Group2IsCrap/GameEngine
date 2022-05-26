#pragma once
#include "Noise.h"
#include "BiomeInfo.h"

#include "..\FirelightEngine\Source\Maths\Rect.h"
#include <Source/Maths/Vec2.h>
#include "..\FirelightEngine\Source\TileMap\TileMap.h"
#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"
#include "Vector3D.h"
#include "Source/ImGuiUI/ImGuiLayer.h"

#include <map>

#define SMALL_FLOAT_VAL 0.4

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
	void Initialise(Firelight::TileMap::TileMap* tileMap, BiomeInfo* biomeInfo);
	void Uninitialise();

	unsigned int CalculateIslandShape(int perlinIndex);

	void GenerateWorld();

	Firelight::Maths::Rectf testPosition;

	bool IsInVoid(Firelight::Maths::Vec2f position);
	void CheckCurrentPlayerBiomeType(Rectf playerPosition);

	void KillVoidTiles();
	Firelight::TileMap::TileMap* GetTileMap();

private:

	void DrawFirstIsland(Rectf& destRect, const Rectf currentIslandCentre, int index);
	void DrawIslandCircles(Rectf& destRect, const Rectf currentIslandCentre, int index);
	void DrawBridge(Rectf& destRect, Rectf currentIslandCentre, IslandSpawnDirection direction);

	bool IsIslandSpaceFree(Vec2i newIslandPosition);

	IslandSpawnDirection CalculateNextIslandDirection(unsigned int noiseIndex);
	unsigned int CalculateRandomIslandIndex();
	void FindNextIslandCentre(Rectf& currentIslandCentre, IslandSpawnDirection& direction, Rectf& destRect, int& iterator);
	BiomeType RandomBiomeType(unsigned int noiseIndex);

	void OutputLowestAndHighestVec(Vec2f& lowestPos, Vec2f& highestPos, Rectf rectVal);
	bool IsPositionBetweenTwoPoints(Rectf position, Vec2f point1, Vec2f point2);

	Noise* m_biomeNoise;
	Noise* m_islandDirectionNoise;
	Noise* m_islandShapeNoise;
	BiomeInfo* m_biomeInfo;

	Firelight::TileMap::TileMap* m_tileMap;
	static BiomeGeneration* sm_instance;

	static unsigned int sm_mapSeed;

	std::vector<Rectf> m_walkableBoxZones;
	std::vector<Rectf> m_islandCentres;

	std::vector< Vec2i > m_OccupiedIslandSpaces;

	Rectf m_initialCentre;

	unsigned int m_biomeCount;
	unsigned int m_bridgeWidth;
	unsigned int m_bridgeLength;
	int m_islandRadii;
	int m_seed;
	int m_numberOfBridges;
	size_t m_numberOfIslands;
	std::vector<bool> m_wasPlayerOnBridges;
	bool m_hasPlayerCollidedRecently;
};
