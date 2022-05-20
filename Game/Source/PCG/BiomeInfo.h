#pragma once

#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"
#include "..\FirelightEngine\Source\ECS\EntityWrappers\Entity.h"

using namespace Firelight::Graphics;
using namespace Firelight::ECS;

enum class BiomeType
{
	Forest,
	Swamp,
	Snow
};

struct BiomeInfo
{
	std::vector<Entity*> possibleSpawnEntities;
	Texture* mainTexture;
	Texture* detailTexture;
};

static std::map<unsigned int, BiomeType> mapOfBiomesOnTileIDs;