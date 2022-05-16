#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include<Source/Maths/Vec2.h>
#include<Source/ECS/Components/UIComponents.h>
#include<vector>

using namespace Firelight::Serialisation;
using CallbackFunctionType = std::function< void() >;
using CallbackFunctionTypeBool = std::function< bool() >;

struct InventoryComponent : Firelight::ECS::BaseComponent
{
	//all
	std::string name;
	Firelight::Maths::Vec2f size;
	Firelight::Maths::Vec2f offset;
	
	UINT slotCount = 0;
	int rowCount;
	int columnCount;
	std::string SlotTextuer;
	bool isDisplay = false;
	bool isSwap= true;

	Firelight::ECS::e_AnchorSettings anchorSettings;

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
	std::string group="NULL";
	bool isDisplayButtons = false;
	bool isDisplayAll = false;

	//key to activate
	int keyToActivate = 0;


	void Serialise() override
	{

	}

	InventoryComponentGroupID* Clone() override
	{
		InventoryComponentGroupID* clone = new InventoryComponentGroupID();

		clone->group = group;
		clone->isDisplayButtons = isDisplayButtons;

		return clone;
	}


};

struct InventoryComponentOutPut: Firelight::ECS::BaseComponent
{
	//special output code
	//eg item to be sold
	std::vector<CallbackFunctionType> outputCommand;
	bool isOutput;
	
	void Serialise() override
	{

	}
};

struct InventoryComponentInPut : Firelight::ECS::BaseComponent
{
	//special input code
	std::vector<CallbackFunctionType> inputCommand;
	bool isInput;

	void Serialise() override
	{

	}

};

struct InventoryComponentEqutmentSlot : Firelight::ECS::BaseComponent
{
	std::string slotName;
	Firelight::Maths::Vec2f offset;
	Firelight::ECS::e_AnchorSettings anchorSettings;
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