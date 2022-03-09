#include "InventoryManager.h"

InventoryManager::InventoryManager(ECS::Entity* parent)
{
    ECS::UIButton* button= new ECS::UIButton();
    button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PlayButton.png");
    button->SetAnchorSettings(ECS::e_AnchorSettings::Left);
    button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
    button->SetOffset(Maths::Vec2f(0.0f, 10.0f));
    button->SetParent(parent->GetEntityID());
    button->GetSpriteComponent()->toDraw=false;
    button->AddComponent<ECS::UIContainerComponent>();
    EntityIDButtion.push_back(button->GetEntityID());

    button = new ECS::UIButton();
    button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PlayButton.png");
    button->SetAnchorSettings(ECS::e_AnchorSettings::Left);
    button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
    button->SetOffset(Maths::Vec2f(0.0f, 200.0f));
    button->SetParent(parent->GetEntityID());
    button->GetSpriteComponent()->toDraw = false;
    button->AddComponent<ECS::UIContainerComponent>();
    EntityIDButtion.push_back(button->GetEntityID());

    Events::EventDispatcher::SubscribeFunction<Events::Inv::UPDATEINV>(std::bind(&InventoryManager::ItemChangeInventory, this));
}

InventoryManager::~InventoryManager()
{
}

void InventoryManager::ItemChangeInventory()
{
    
    for (auto In : m_Inventory) {
        for (auto inv: In.second)
        {
            if (inv->GetNullSlotData()->size() == 0) {
                continue;
            }
            for (auto Slot : *inv->GetNullSlotData()) {
                //move
                //find
                for (auto In2 : m_Inventory) {
                    for (auto inv2 : In.second)
                    {
                       std::string a = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot->UITexID)->parentID)->buttonText;
                       if (inv2->GetName() == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot->UITexID)->parentID)->buttonText) {
                            inv2->AddItem(Slot);
                       }
                    }
                }
            }
            inv->GetNullSlotData()->clear();
        }
    }
    //drop code here

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
        int index = 0;
        for (auto* In : m_Inventory[group]) {
            if (!In->GetIsDisplay()) {

                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(EntityIDButtion[index])->buttonText = In->GetName();
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(EntityIDButtion[index])->toDraw = true;
                for (size_t i = 0; i < m_Inventory[group].size(); i++)
                {
                    if (i == index) {
                        continue;
                    }
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::UnloadInventory, m_Inventory[group][i]));
                }
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::LoadInventory,In,&EntityIDPannlSlot,false));
                if (index == 0) {
                    In->LoadInventory(&EntityIDPannlSlot, false);
                }
                index++;
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
    int index = 0;
      for (auto In : m_Inventory[group]) {
          
          ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(EntityIDButtion[index])->buttonText ="null";
          ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.clear();
          ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(EntityIDButtion[index])->toDraw=false;
          In->UnloadInventory();
          index++;
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
