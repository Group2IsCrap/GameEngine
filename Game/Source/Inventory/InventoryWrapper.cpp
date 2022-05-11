#include "InventoryWrapper.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"
InventoryWrapper::InventoryWrapper()
{
    
    AddComponent<InventoryComponentGroupID>();
    
}

InventoryWrapper::InventoryWrapper(std::string Name, bool isDisplayButtions, bool isDisplayAll,Firelight::Keys keyToAcvate)
{
    AddComponent<InventoryComponentGroupID>();
    
    GetInvGroup()->Group = Name;
    GetInvGroup()->isDisplayButtions = isDisplayButtions;
    GetInvGroup()->isDisplayAll = isDisplayAll;
    GetInvGroup()->keyToAcvate = keyToAcvate;
    //add inv
    Firelight::Events::EventDispatcher::InvokeFunctions <Firelight::Events::Inv::ADD_NEW_INV>();
    
}

InventoryWrapper::~InventoryWrapper()
{
}

void InventoryWrapper::AddInventory(std::string Name, int RowCount, int ColoumCount, Firelight::Maths::Vec2f Size, Firelight::Maths::Vec2f OffSet, Firelight::ECS::e_AnchorSettings UIAnchor)
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentButtonLayout>();
    GetInvGroup()->NumberOfInvetorys++;

    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->Name = Name;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->offset = OffSet;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->Size = Size;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->RowCount = RowCount;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->ColoumCount = ColoumCount;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->AnchorSettings = UIAnchor;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->SlotCount = RowCount * ColoumCount;
    this->GetInvComp(GetInvGroup()->NumberOfInvetorys - 1)->SlotStartPositon = GetInvGroup()->TotalNumberOfSlots;

    int index = GetInvGroup()->TotalNumberOfSlots;
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
    GetInvGroup()->TotalNumberOfSlots = index;
    //add inv
    Firelight::Events::EventDispatcher::InvokeFunctions <Firelight::Events::Inv::ADD_NEW_INV>();
}
