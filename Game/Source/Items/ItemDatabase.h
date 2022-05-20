#pragma once
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/EntityWrappers/Template.h>

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

private:
	ItemDatabase();
	~ItemDatabase();

	void LoadExtra(std::string filepath, std::map<int, std::vector<std::string>>& Data);
	void AddExtraComponent(Template* itemTemplate, std::map<int, std::vector<std::string>>weaponData, std::map<int, std::vector<std::string>>armourData, std::map<int, std::vector<std::string>>foodData);
	static ItemDatabase* sm_instance;

	std::unordered_map<int, Template*> itemTemplates;

};

