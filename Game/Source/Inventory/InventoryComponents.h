#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include<Source/Maths/Vec2.h>
#include<Source/ECS/Components/UIComponents.h>
#include<vector>
#include<Source/KeyBinder.h>
using namespace Firelight::Serialisation;
using namespace Firelight::ECS;
using CallbackFunctionType = std::function< void() >;
using CallbackFunctionTypeBool = std::function< bool() >;

/// <summary>
/// main component data
/// </summary>
struct InventoryComponent : Firelight::ECS::BaseComponent
{
	//all
	std::string Name;
	Firelight::Maths::Vec2f Size;
	Firelight::Maths::Vec2f offset;
	Firelight::ECS::e_AnchorSettings AnchorSettings;

	std::string BackgroundTex;
	std::string SlotTex;

	UINT SlotCount = 0;
	int RowCount=0;
	int ColoumCount=0;

	bool isDisplay = false;
	bool isSwap= true;

	void Serialise() override
	{
		
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
	std::string Group="NULL";
	bool isDisplayButtions = false;
	bool isDisplayAll = false;
	Firelight::Keys keyToAcvate = Firelight::Keys::KEY_INVALID;
	
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
/// <summary>
/// single slot
/// </summary>
struct InventorySlots : Firelight::ECS::BaseComponent
{
	//slotData
	int CurrIndex = 0;
	EntityID SlotID = NULL;
	bool IsUsed = false;

	void Serialise() override
	{
		//to store
		/*int CurrPos = 0;
		bool IsUsed = false;*/
	}
};
/// <summary>
/// data for the slot
/// </summary>
struct InventoryStoreData : Firelight::ECS::BaseComponent
{
	//stored items
	int SlotIndex;
	int StackSize = -1;
	std::vector<EntityID> EntityIDs;
	EntityID UITexID;


	void Serialise() override
	{
		//to store
		/*SlotIndex
		StackSize
		EntityIDs*/
	}
};

/// <summary>
/// 
/// </summary>
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

/// <summary>
/// 
/// </summary>
struct InventoryComponentInPut : Firelight::ECS::BaseComponent
{
	//special input code
	std::vector<CallbackFunctionType> InputCommand;
	bool isInput;

	void Serialise() override
	{

	}

};

/// <summary>
/// specilal slot
/// </summary>
struct InventoryComponentEqutmentSlot : Firelight::ECS::BaseComponent
{
	std::string SlotName;
	Firelight::Maths::Vec2f OffSet;
	Firelight::ECS::e_AnchorSettings AnchorSettings;
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
