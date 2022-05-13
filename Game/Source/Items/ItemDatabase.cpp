#include "ItemDatabase.h"

#include <Source/ECS/EntityComponentSystem.h>
#include <Source/ECS/Components/ItemComponents.h>
#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/Graphics/AssetManager.h>

#include <fstream>
#include <sstream>
#include <vector>

using namespace Firelight;

ItemDatabase* ItemDatabase::sm_instance = nullptr;

ItemDatabase* ItemDatabase::Instance()
{
	if (sm_instance == nullptr)
	{
		sm_instance = new ItemDatabase();
	}
	return sm_instance;
}

void ItemDatabase::LoadItems(std::string filepath)
{
	std::vector<std::vector<std::string>> itemData;
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
			itemData.push_back(row);
		}
	}

	for (int i = 0; i < itemData.size(); ++i)
	{
		Template* itemTemplate = new Template();
		ItemComponent* itemComponent = itemTemplate->AddComponent<ItemComponent>();
		itemComponent->itemID = std::stoi(itemData[i][0]);
		itemComponent->name = itemData[i][1];
		itemComponent->description = itemData[i][2];
		itemComponent->iconPath = itemData[i][3];
		itemComponent->stackSize = std::stoi(itemData[i][4]);
		SpriteComponent* spriteComponent = itemTemplate->AddComponent<SpriteComponent>();
		spriteComponent->texture = Graphics::AssetManager::Instance().GetTexture(itemComponent->iconPath);
		spriteComponent->pixelsPerUnit = 50;
		spriteComponent->layer = 33;
		TransformComponent* transformComponent = itemTemplate->AddComponent<TransformComponent>();
		StaticComponent* staticComponent = itemTemplate->AddComponent<StaticComponent>();
		staticComponent->isStatic = false;
		CircleColliderComponent* circleCollider = new CircleColliderComponent();
		itemTemplate->AddComponent<Firelight::ECS::ColliderComponent>(circleCollider);
		LayerComponent* layerComponent = itemTemplate->AddComponent<LayerComponent>();

		itemTemplates.insert(std::make_pair(itemComponent->itemID, itemTemplate));
	}
}

Entity* ItemDatabase::CreateInstanceOfItem(int itemID)
{
	Entity* entity = new Entity(true, itemTemplates[itemID]->GetTemplateID());		
	entity->GetComponent<TransformComponent>()->position.x = (float)(rand() % 20) - 10.0f;
	entity->GetComponent<TransformComponent>()->position.y = (float)(rand() % 20) - 10.0f;
	entity->GetComponent<LayerComponent>()->layer = 1;
	entity->GetComponent<ColliderComponent, CircleColliderComponent>()->radius = 1.0f;
	entity->GetComponent<ColliderComponent, CircleColliderComponent>()->isTrigger = true;

	return entity;
}

ItemDatabase::ItemDatabase()
{
	itemTemplates = std::unordered_map<int, Template*>();
}

ItemDatabase::~ItemDatabase()
{
	for (auto &templatePair : itemTemplates)
	{
		delete templatePair.second;
	}

	itemTemplates.clear();
}
