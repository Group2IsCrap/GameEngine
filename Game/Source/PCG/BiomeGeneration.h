#pragma once
#include "Noise.h"
#include "Biomes.h"
#include <map>

class BiomeGeneration
{
public:
	BiomeGeneration();

	static BiomeGeneration* Instance();
	void Initialise();
	void Uninitialise();

	size_t RandomBiomeIndex(unsigned int perlinIndex);
	size_t CalculateIslandShape(int perlinIndex);

	void Draw();
	void Render();

private:

	Noise* m_biomeNoise;
	Noise* m_islandShapeNoise;

	static BiomeGeneration* sm_instance;
	static unsigned int mapSeed;

	std::vector< Firelight::Graphics::Texture*> sm_biomeMap;
	Biome m_biome;
};
