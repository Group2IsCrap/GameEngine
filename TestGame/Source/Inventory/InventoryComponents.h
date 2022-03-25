#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include<Source/Maths/Vec2.h>

#include<vector>

using namespace Firelight::Serialisation;
using CallbackFunctionType = std::function< void() >;


struct InventoryComponent : Firelight::ECS::BaseComponent
{
	//all
	std::string Name;
	Firelight::Maths::Vec2i Size;
	UINT SlotCount = 0;
	int RowCount;
	int ColoumCount;
	std::string SlotTextuer;
	bool isDisplay = false;
	bool isSwap= true;

	//stored items
	std::vector<Firelight::ECS::EntityID> StoredItems;

	void Serialise() override
	{
		
	}

	InventoryComponent* Clone() override
	{
		InventoryComponent* clone = new InventoryComponent();
		
		return clone;
	}

};


struct InventoryComponentGroupID : Firelight::ECS::BaseComponent
{
	std::string Group="NULL";
	bool isDisplayButtions = false;

	//key to activate
	int keyToAcvate = 0;


	void Serialise() override
	{

	}

	InventoryComponentGroupID* Clone() override
	{
		InventoryComponentGroupID* clone = new InventoryComponentGroupID();

		clone->Group = Group;
		clone->isDisplayButtions = isDisplayButtions;

		return clone;
	}


};

struct InventoryComponentOutPut: Firelight::ECS::BaseComponent
{
	//special output code
	//eg item to be sold
	std::vector<CallbackFunctionType> OutputCommand;
	bool isOutput;

	void Serialise() override
	{

	}
};

struct InventoryComponentInPut : Firelight::ECS::BaseComponent
{
	//special input code
	std::vector<CallbackFunctionType> InputCommand;
	bool isInput;

	void Serialise() override
	{

	}

};

struct InventoryComponentEqutmentSlot : Firelight::ECS::BaseComponent
{
	std::string SlotName;
	Firelight::Maths::Vec2i Pos;
};


struct InventoryComponentButtionLayout : Firelight::ECS::BaseComponent
{

};




struct InventoryComponentManager : Firelight::ECS::BaseComponent
{
	Firelight::ECS::EntityID ParentID;
	int PreCreatedButtions;
	int PreCreatedSlots;


};