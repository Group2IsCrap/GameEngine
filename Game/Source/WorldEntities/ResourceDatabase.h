#pragma once
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/EntityWrappers/Template.h>

#include "../CoreComponents/ResourceComponent.h"

#include "../Core/ResourceEntity.h"

#include <string>
#include <unordered_map>

using namespace Firelight::ECS;

class ResourceDatabase
{
public:
	static ResourceDatabase* Instance();

	void LoadResources(std::string filepath);

	ResourceEntity* CreateInstanceOfResource(int resourceID);

private:
	ResourceDatabase();
	~ResourceDatabase();
	void AddColliders(Template* resourceTemplate, std::string colliderData);
	void AddDrops(ResourceComponent* resourceComponent, std::string dropData);

	static ResourceDatabase* sm_instance;

	std::unordered_map<int, Template*> resourceTemplates;

};

