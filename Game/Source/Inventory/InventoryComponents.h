#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include<Source/Maths/Vec2.h>
#include<Source/ECS/Components/UIComponents.h>
#include<vector>
#include<Source/KeyBinder.h>
using namespace Firelight::Serialisation;
using namespace Firelight::ECS;
using CallbackFunctionTypeVoid = std::function< void(void*) >;
using CallbackFunctionTypeBool = std::function< bool() >;

/// <summary>
/// main component data
/// </summary>
struct InventoryComponent : Firelight::ECS::BaseComponent
{
	//all
	std::string name;

	//global positonal settings
	Firelight::Maths::Vec2f size;
	Firelight::Maths::Vec2f offset;
	Firelight::ECS::e_AnchorSettings anchorSettings;

	//textuer
	std::string backgroundTexture= "Sprites/PanelTest.png";
	std::string slotTexture= "Sprites/Slot_Icon_100x100.png";

	//slot data
	UINT slotCount = 0;
	int rowCount=0;
	int columnCount=0;
	int slotStartPositon = 0;

	//controll data
	bool isDisplay = false;
	bool isSwap= true;

	void Serialise() override
	{
		
		Serialiser::Serialise("name", name);

		Serialiser::Serialise("size", size);
		Serialiser::Serialise("offset", offset);
		Serialiser::Serialise("AnchorSettings", (int)anchorSettings);

		Serialiser::Serialise("backgroundTexture", backgroundTexture);
		Serialiser::Serialise("slotTexture", slotTexture);
		
		Serialiser::Serialise("slotCount", slotCount);
		Serialiser::Serialise("rowCount", rowCount);
		Serialiser::Serialise("columnCount", columnCount);
		Serialiser::Serialise("slotStartPositon", slotStartPositon);

		Serialiser::Serialise("isDisplay", isDisplay);
		Serialiser::Serialise("isSwap", isSwap);
	}

	InventoryComponent* Clone() override
	{
		InventoryComponent* clone = new InventoryComponent();
		
		return clone;
	}

};

/// <summary>
/// Group information
/// </summary>
struct InventoryComponentGroupID : Firelight::ECS::BaseComponent
{
	std::string group="NULL";
	bool isDisplayButtons = false;
	bool isDisplayAll = false;
	Firelight::Keys keyToActivate = Firelight::Keys::KEY_INVALID;
	int numberOfInventories = 0;
	int totalNumberOfSlots = 0;
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
/// <summary>
/// single slot
/// </summary>
struct InventorySlots : Firelight::ECS::BaseComponent
{
	//slotData
	int currentIndex = 0;
	EntityID slotID = NULL;
	bool isUsed = false;
	int specialSlotIndex = -1;
	void Serialise() override
	{
		//to store
		/*int CurrPos = 0;
		bool isUsed = false;*/
	}
};
/// <summary>
/// data for the slot
/// </summary>
struct InventoryStoreData : Firelight::ECS::BaseComponent
{
	//stored items
	int slotIndex;
	int stackSize = -1;
	std::vector<EntityID> entityIDs;
	EntityID UITexID;


	void Serialise() override
	{
		//to store
		/*SlotIndex
		stackSize
		entityIDs*/
	}
};

/// <summary>
/// when the slot data exits all inventorys do comnands
/// </summary>
struct InventoryComponentOutPut: Firelight::ECS::BaseComponent
{
	//special output code
	//eg item to be sold
	std::vector<CallbackFunctionTypeVoid> outputCommand;
	bool isOutput;
	
	void Serialise() override
	{

	}
};

/// <summary>
/// 
/// </summary>
struct InventoryComponentInPut : Firelight::ECS::BaseComponent
{
	//special input code
	std::vector<CallbackFunctionTypeVoid> inputCommand;
	bool isInput;

	void Serialise() override
	{

	}

};

/// <summary>
/// specilal slot
/// creator is responsible for overlaps and if it looks bad
/// </summary>
struct InventoryComponentSpecialSlot : Firelight::ECS::BaseComponent
{
	std::string slotName;
	Firelight::Maths::Vec2f offset;
	Firelight::Maths::Vec2f size;
	Firelight::ECS::e_AnchorSettings anchorSettings;
	std::vector<std::string> tags;

	void Serialise() override
	{

	}
};

struct InventoryComponentButtonLayout : Firelight::ECS::BaseComponent
{
	std::string buttonTexture;

	Firelight::Maths::Vec2f offset;
	Firelight::ECS::e_AnchorSettings anchorSettings;
	bool toUse = false;

	void Serialise() override
	{

	}
};


