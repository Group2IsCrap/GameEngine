#pragma once

#include "..\FirelightEngine\Source\Graphics\Data\Texture.h"
#include "..\FirelightEngine\Source\ECS\EntityWrappers\Entity.h"

struct BiomeInfo
{
	std::vector<Firelight::ECS::Entity*> possibleSpawnEntities;
	Firelight::Graphics::Texture* mainTexture;
	Firelight::Graphics::Texture* detailTexture;
};