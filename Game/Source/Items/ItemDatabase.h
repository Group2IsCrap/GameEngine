#pragma once
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/EntityWrappers/Template.h>
#include <Source/ECS/Components/ItemComponents.h>

#include <string>
#include <unordered_map>

using namespace Firelight::ECS;

class ItemDatabase
{
public:
	static ItemDatabase* Instance();

	void LoadItems(std::string filepath);

	std::vector<std::string> GetTagList(std::string stream);

	Entity* CreateInstanceOfItem(int itemID);

	// Bad, should probably be optimised in some way
	int            GetItemIdFromName(const std::string& name);
	Template*      GetItemTemplateFromId(int itemId);
	ItemComponent* GetItemComponentFromId(int itemId);

private:
	ItemDatabase();
	~ItemDatabase();

	void LoadExtra(std::string filepath, std::map<int, std::vector<std::string>>& Data);
	void AddExtraComponent(Template* itemTemplate, std::map<int, std::vector<std::string>>weaponData, std::map<int, std::vector<std::string>>armourData, std::map<int, std::vector<std::string>>foodData);
	static ItemDatabase* sm_instance;

	std::unordered_map<int, Template*> itemTemplates;

};

