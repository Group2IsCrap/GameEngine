#include "InventoryManager.h"

InventoryManager::InventoryManager()
{
}

InventoryManager::~InventoryManager()
{
}

void InventoryManager::ItemChangeInventory()
{
}

void InventoryManager::CreatInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::Entity* parent)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreatInventoryNoPannel(size, columnRows, parent);
    m_Inventory[group].emplace_back(newInv);
}

void InventoryManager::CreatInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::Entity* parent)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreatInventoryNoPannel(size, slotCont, parent);
    m_Inventory[group].emplace_back(newInv);
}

void InventoryManager::LoadInventory(GroupName group, std::string name)
{
    //create Group
   for (auto In : m_Inventory[group]) {
        if (!In->GetIsDisplay() && In->GetName()== name) {
                In->LoadInventory(&EntityIDPannlSlot,false);
          }
   }
 
}

void InventoryManager::LoadInventoryGroup(std::string group)
{
        //create Group
        for (auto In : m_Inventory[group]) {
            if (!In->GetIsDisplay()) {
                In->LoadInventory(&EntityIDPannlSlot,false);
            }
        }
}

void InventoryManager::UnloadInventory(GroupName group, std::string name)
{
    //unloade Group
    for (auto In : m_Inventory[group]) {
        if (In->GetIsDisplay() && In->GetName() == name) {
            In->UnloadInventory();
        }
    }
}

void InventoryManager::UnloadInventoryGroup(std::string group)
{
      //unloade Group
      for (auto In : m_Inventory[group]) {
          if (In->GetIsDisplay()) {
              In->UnloadInventory();
          }
      }
}

void InventoryManager::AddItem(GroupName group,std::string Name, Firelight::ECS::Entity* item)
{
    //Add Item
    for (auto In : m_Inventory[group]) {
        if (In->GetName() == Name) {
            In->AddItem(item);
            break;
        }
    }
}

void InventoryManager::RemoveItem(GroupName group, std::string Name, Firelight::ECS::Entity* item)
{
    for (auto In : m_Inventory[group]) {
        if (In->GetName() == Name) {
            In->RemoveItem(item);
            break;
        }
    }
}

void InventoryManager::RemoveItem(GroupName group, std::string Name, int item, int howMany)
{
    for (auto In : m_Inventory[group]) {
        if (In->GetName() == Name) {
            In->RemoveItemType(howMany, item);
            break;
        }
    }
}

std::vector<ECS::EntityID> InventoryManager::GetItems(GroupName group, std::string Name, int item, int howMany)
{
    for (auto In : m_Inventory[group]) {
        if (In->GetName() == Name) {
            return In->GetItemType(howMany, item); 
        }
    }
}

bool InventoryManager::CheckInventory(ECS::EntityID ID, std::string InvName, GroupName Group)
{
    return false;
}
