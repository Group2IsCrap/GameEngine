#include "ItemDatabase.h"

#include <Source/ECS/EntityComponentSystem.h>
#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/ECS/Components/AudioComponents.h>
#include <Source/Graphics/AssetManager.h>
#include "../Core/Layers.h"

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
		itemComponent->tags = GetTagList(itemData[i][5]);
		SpriteComponent* spriteComponent = itemTemplate->AddComponent<SpriteComponent>();
		spriteComponent->texture = Graphics::AssetManager::Instance().GetTexture(itemComponent->iconPath);
		spriteComponent->pixelsPerUnit = 50;
		spriteComponent->layer = static_cast<int>(RenderLayer::Items);
		TransformComponent* transformComponent = itemTemplate->AddComponent<TransformComponent>();
		StaticComponent* staticComponent = itemTemplate->AddComponent<StaticComponent>();
		staticComponent->isStatic = false;
		CircleColliderComponent* circleCollider = new CircleColliderComponent();
		itemTemplate->AddComponent<Firelight::ECS::ColliderComponent>(circleCollider);
		LayerComponent* layerComponent = itemTemplate->AddComponent<LayerComponent>();
		layerComponent->layer = static_cast<int>(GameLayer::Items);
		AudioComponent* audioComponent = new AudioComponent();
		itemTemplate->AddComponent<Firelight::ECS::AudioComponent>(audioComponent);
		audioComponent->soundName = "beeuuuuu.mp3";
		audioComponent->soundPos = Vector3D(0, 0, 0);
		audioComponent->looping = false;
		audioComponent->is3d = false;
		audioComponent->streaming = false;
		audioComponent->channel = "UI";


		itemTemplates.insert(std::make_pair(itemComponent->itemID, itemTemplate));
	}
}

std::vector<std::string> ItemDatabase::GetTagList(std::string stream)
{
	std::vector<std::string> tagList;

	if (stream.length() > 0)
	{
		while (stream.find(";") != std::string::npos)
		{
			std::string left = stream.substr(0, stream.find_first_of(";"));
			std::string right = stream.substr(stream.find_first_of(";") + 1);

			tagList.push_back(left);
			stream = right;
		}
		tagList.push_back(stream);
	}

	return tagList;
		
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

// Bad, should probably be optimised in some way
int ItemDatabase::GetItemIdFromName(const std::string& name)
{
	for (auto& mapIter : itemTemplates)
	{
		if (ItemComponent* itemComponent = mapIter.second->GetComponent<ItemComponent>())
		{
			if (itemComponent->name == name)
			{
				return itemComponent->itemID;
			}
		}
	}

	return -1;
}

Template* ItemDatabase::GetItemTemplateFromId(int itemId)
{
	auto mapIter = itemTemplates.find(itemId);
	if (mapIter != itemTemplates.end())
	{
		return mapIter->second;
	}
	return nullptr;
}

ItemComponent* ItemDatabase::GetItemComponentFromId(int itemId)
{
	if (Template* itemTemplate = GetItemTemplateFromId(itemId))
	{
		return itemTemplate->GetComponent<ItemComponent>();
	}
	return nullptr;
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
