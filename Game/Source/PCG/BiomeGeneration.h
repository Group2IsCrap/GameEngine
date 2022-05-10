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

	size_t RandomBiomeIndex(int perlinIndex);

	void Draw();
	void Render();

private:
	Noise* m_noise;

	static BiomeGeneration* sm_instance;


	//list of biomes to get their textures

	std::vector< Firelight::Graphics::Texture*> sm_biomeMap;
	Biome m_biome;

	//Array of biomes
};
