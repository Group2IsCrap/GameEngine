#pragma once

#include <Source/ECS/Components/UIComponents.h>
#include <Source/ECS/EntityWrappers/UIEntity.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/ECS/Systems/UISystem.h>
#include<Source/Maths/Vec2.h>
#include <string>
#include <vector>

using namespace Firelight;
class Inventory
{
public:
	Inventory();
	~Inventory();

	void CreatInventory(Maths::Vec2f size, float slotCount,ECS::Entity* parent);
	void CreatInventory(Maths::Vec2f size, Maths::Vec2f rows, ECS::Entity* parent);

	void AddItem(Firelight::ECS::Entity* item);
	void RemoveItem(Firelight::ECS::Entity* item);

private:



public:


private:

	std::string m_Name;

	bool isOutput;
	bool isInput;
	bool isSwap;

	UINT SlotCount;

	Maths::Vec2i Size;
	std::vector<std::vector<ECS::UIPanel>> Grid;
	std::vector<std::vector<ECS::Entity>> StoredItems;

};

