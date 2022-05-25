#pragma once

#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"
#include "..\FirelightEngine\Source\ECS\EntityWrappers\Entity.h"

using namespace Firelight::Graphics;
using namespace Firelight::ECS;

enum class BiomeType
{
	Void,
	Forest,
	Swamp,
	Snow,
	Bridge
};

struct BiomeInfo
{
	std::map<unsigned int, BiomeType> mapOfBiomesOnTileIDs;
};

struct BiomeMusicData
{
	Vector3D playerPosition;
	BiomeType biome;
};

static std::vector< Firelight::Graphics::Texture* > sm_biomeTextures;