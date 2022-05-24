#include "ResourceDatabase.h"

#include <Source/ECS/EntityComponentSystem.h>

#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/ECS/Components/AudioComponents.h>
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>
#include <Source/Graphics/AssetManager.h>
#include "../Core/Layers.h"


#include "../CoreComponents/HealthComponent.h"



#include <fstream>
#include <sstream>
#include <vector>

using namespace Firelight;

ResourceDatabase* ResourceDatabase::sm_instance = nullptr;

ResourceDatabase* ResourceDatabase::Instance()
{
	if (sm_instance == nullptr)
	{
		sm_instance = new ResourceDatabase();
	}
	return sm_instance;
}

void ResourceDatabase::LoadResources(std::string filepath)
{
	std::vector<std::vector<std::string>> resourceData;
	std::vector<std::string> row;
	std::string line, column;

	std::fstream file(filepath, std::ios::in);
	if (file.is_open())
	{
		getline(file, line);
		while (getline(file, line))
		{
			row.clear();

			std::stringstream str(line);

			while (getline(str, column, ','))
			{
				row.push_back(column);
			}
			resourceData.push_back(row);
		}
	}
	file.close();

	for (int i = 0; i < resourceData.size(); ++i)
	{
		Template* resourceTemplate = new SpriteEntityTemplate("Resource Template - " + resourceData[i][1]);

		SpriteComponent* spriteComponent = resourceTemplate->GetComponent<SpriteComponent>();
		if (resourceData[i][2] != "")
		{
			spriteComponent->texture = Graphics::AssetManager::Instance().GetTexture(resourceData[i][2]);
		}
		spriteComponent->pixelsPerUnit = std::stoi(resourceData[i][6]);
		int layer = std::stoi(resourceData[i][7]);
		spriteComponent->layer = layer == 0 ? static_cast<int>(RenderLayer::Items) : layer;

		ResourceComponent* resourceComponent = resourceTemplate->AddComponent<ResourceComponent>();
		resourceComponent->resourceID = std::stoi(resourceData[i][0]);

		resourceComponent->name = resourceData[i][1];

		AddColliders(resourceTemplate, resourceData[i][5]);


		AddDrops(resourceComponent, resourceData[i][4]);

		resourceTemplate->AddComponent<HealthComponent>()->currentHealth = std::stoi(resourceData[i][3]);
		resourceTemplate->AddComponent<RigidBodyComponent>();

		resourceTemplates.insert(std::make_pair(resourceComponent->resourceID, resourceTemplate));

	}
}

ResourceEntity* ResourceDatabase::CreateInstanceOfResource(int resourceID)
{
	ResourceEntity* entity = new ResourceEntity(true, resourceTemplates[resourceID]->GetTemplateID());
	entity->GetComponent<LayerComponent>()->layer = 3;
	entity->GetComponent<IdentificationComponent>()->name = "Resource: " + entity->GetComponent<ResourceComponent>()->name;

	return entity;
}

ResourceDatabase::ResourceDatabase()
{
	resourceTemplates = std::unordered_map<int, Template*>();
}

ResourceDatabase::~ResourceDatabase()
{
	for (auto& templatePair : resourceTemplates)
	{
		delete templatePair.second;
	}

	resourceTemplates.clear();
}

void ResourceDatabase::AddColliders(Template* resourceTemplate, std::string colliderData)
{
	if (colliderData.length() > 0)
	{
		while (colliderData.find(";") != std::string::npos)
		{
			std::string left = colliderData.substr(0, colliderData.find_first_of(";"));
			std::string right = colliderData.substr(colliderData.find_first_of(";") + 1);

			std::string type = left.substr(0, left.find_first_of(":"));
			std::string data = left.substr(left.find_first_of(":") + 1);

			std::vector<std::string> colliderParams;

			std::stringstream str(data);

			std::string currentParam;

			while (getline(str, currentParam, ':'))
			{
				colliderParams.push_back(currentParam);
			}

			if (type == "Box")
			{
				BoxColliderComponent* boxCollider = new BoxColliderComponent();

				boxCollider->rect = Maths::Rectf(std::stof(colliderParams[0]), std::stof(colliderParams[1]), std::stof(colliderParams[2]), std::stof(colliderParams[3]));
				boxCollider->isTrigger = static_cast<bool>(std::stoi(colliderParams[4]));

				resourceTemplate->AddComponent<ColliderComponent>(boxCollider);
			}
			else
			{
				CircleColliderComponent* circleCollider = new CircleColliderComponent();

				circleCollider->radius = std::stof(colliderParams[0]);
				circleCollider->offset = Maths::Vec2f(std::stof(colliderParams[1]), std::stof(colliderParams[2]));
				circleCollider->isTrigger = static_cast<bool>(std::stoi(colliderParams[3]));
				resourceTemplate->AddComponent<ColliderComponent>(circleCollider);
			}

			colliderData = right;
		}
	}

}

void ResourceDatabase::AddDrops(ResourceComponent* resourceComponent, std::string dropData)
{
	if (dropData.length() > 0)
	{
		while (dropData.find(";") != std::string::npos)
		{
			std::string left = dropData.substr(0, dropData.find_first_of(";"));
			std::string right = dropData.substr(dropData.find_first_of(";") + 1);

			std::vector<std::string> dropParams;

			std::stringstream str(left);

			std::string currentParam;

			while (getline(str, currentParam, ':'))
			{
				dropParams.push_back(currentParam);
			}

			ItemDrops drop;

			drop.itemID = std::stoi(dropParams[0]);
			drop.chance = std::stoi(dropParams[1]);
			drop.minDrop = std::stoi(dropParams[2]);
			drop.maxDrop = std::stoi(dropParams[3]);

			resourceComponent->itemDrops.push_back(drop);

			dropData = right;
		}
	}
}