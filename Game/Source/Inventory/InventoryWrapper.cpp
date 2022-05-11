#include "InventoryWrapper.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"
InventoryWrapper::InventoryWrapper()
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentGroupID>();
    AddComponent<InventoryComponentButtonLayout>();
}

InventoryWrapper::InventoryWrapper(std::string Name, int RowCount, int ColoumCount, Firelight::Maths::Vec2f Size, Firelight::Maths::Vec2f OffSet, Firelight::ECS::e_AnchorSettings UIAnchor)
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentGroupID>();
    AddComponent<InventoryComponentButtonLayout>();

    this->GetInvComp()->Name = Name;
    this->GetInvComp()->offset = OffSet;
    this->GetInvComp()->Size = Size;
    this->GetInvComp()->RowCount = RowCount;
    this->GetInvComp()->ColoumCount = ColoumCount;
    this->GetInvComp()->AnchorSettings = UIAnchor;
    this->GetInvComp()->SlotCount = RowCount * ColoumCount;
    int index = 0;
    for (size_t i = 0; i < RowCount; i++)
    {
        for (size_t i = 0; i < ColoumCount; i++)
        {
            AddComponent<InventorySlots>();
            AddComponent<InventoryStoreData>();

            //pair up
            GetComponent<InventorySlots>(index)->CurrIndex = index;
            GetComponent<InventoryStoreData>(index)->SlotIndex = index;
            index++;
        }

    }

    //add inv
    Firelight::Events::EventDispatcher::InvokeFunctions <Firelight::Events::Inv::ADD_NEW_INV>();
    
}

InventoryWrapper::~InventoryWrapper()
{
}

void InventoryWrapper::SetGroup(InventoryComponentGroupID Group)
{
    GetComponent<InventoryComponentGroupID>()->Group = Group.Group;
    GetComponent<InventoryComponentGroupID>()->isDisplayButtions = Group.isDisplayButtions;
    GetComponent<InventoryComponentGroupID>()->isDisplayAll = Group.isDisplayAll;
    GetComponent<InventoryComponentGroupID>()->keyToAcvate = Group.keyToAcvate;

    Firelight::Events::EventDispatcher::InvokeFunctions <Firelight::Events::Inv::ADD_NEW_INV>();
}

void InventoryWrapper::SetGroup(InventoryComponentGroupID* Group)
{
    RemoveComponent<InventoryComponentGroupID>();
    AddComponent<InventoryComponentGroupID>(Group);
}
