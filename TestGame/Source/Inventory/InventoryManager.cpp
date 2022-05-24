#include "InventoryManager.h"
#include"InventoryComponents.h"
#include"Source/ECS/ECSEvents.h"
InventoryManager::InventoryManager()
{
    AddWhitelistComponent<InventoryComponent>();
    AddWhitelistComponent<InventoryComponentGroupID>();

    //sub to this event
    Events::EventDispatcher::SubscribeFunction<Events::Inventory::UpdateInventory>(std::bind(&InventoryManager::ItemChangeInventory, this));
    Events::EventDispatcher::SubscribeFunction<Events::Inventory::AddNewInventory>(std::bind(&InventoryManager::CreateNewInventory, this));

    //Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(std::bind(&InventoryManager::CreateNewInventory, this));
    //Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityDestroyedEvent>(std::bind(&InventoryManager::RemoveInventory, this));

    Events::EventDispatcher::AddListener<Events::Inventory::LoadInventoryGroup>(this);
    Events::EventDispatcher::AddListener<Events::Inventory::UnloadInventoryGroup>(this);
}

InventoryManager::~InventoryManager()
{
}

void InventoryManager::HandleEvents(const char* event, void* data)
{
    if (event == Events::Inventory::LoadInventoryGroup::sm_descriptor) {
        const char* ab = (const char*)data;
       
        LoadInventoryGroup(ab);
    }
    else if(event == Events::Inventory::UnloadInventoryGroup::sm_descriptor)
    {
        UnloadInventoryGroup(*(std::string*)data);
    }

}



void InventoryManager::CreateNewInventory()
{
    for (size_t i = 0; i < m_entities.size(); i++)
    {
        ECS::EntityID ab = m_entities[i]->GetEntityID();
        InventoryComponent* a = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(ab);
        InventoryComponentGroupID* b = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(ab);
        bool toAdd = true;
        for (auto In : m_inventory[b->group]) {
            if ( In->GetName() == a->name) {
                toAdd = false;
            }
        }

        if (toAdd) {
            CreateInventory(b->group, a->name, a->size, Maths::Vec2f(a->columnCount, a->rowCount), ParentID, a->offset,a->anchorSettings);
            m_inventory[b->group].back()->SetEntityData(m_entities.back()->GetEntityID());
        }
    }
}

void InventoryManager::RemoveInventory()
{



}

void InventoryManager::ItemChangeInventory()
{
    //cheage inventorys
    bool toDrop = true;
    //find unplaced slots in all invetorys
    for (auto In : m_inventory) {
        for (auto inv: In.second)
        {
            if (inv->GetNullSlotData()->size() == 0) {
                continue;
            }
            for (auto Slot : *inv->GetNullSlotData()) {
                //find
                for (auto In2 : m_inventory) {
                    for (auto inv2 : In2.second)
                    {
                        //move to new invetory
                        //via buttion text
                       if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot->UITexID)->parentID)) {
                           if (inv2->GetName() == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot->UITexID)->parentID)->buttonText) {
                               toDrop =  inv2->AddItem(Slot, false);
                               if (!toDrop) {
                                   inv->GetNullSlotData()->clear();
                                   return;
                               }
                           }
                       }
                       //via parent ids from ECS
                       else if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot->UITexID)->parentID)->parentID == inv2->GetInventorySpace()->GetEntityID()) {
                           toDrop = inv2->AddItem(Slot,true);
                           if (!toDrop) {
                               inv->GetNullSlotData()->clear();
                               return;
                           }
                       }

                    }
                }
            }
            inv->GetNullSlotData()->clear();
        }
    }
   
    if (toDrop) {
         //drop code here
    }

}

void InventoryManager::CreateInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreateInventory(size, columnRows, parent, anc, offSet);
    m_inventory[group].emplace_back(newInv);
}

void InventoryManager::CreateInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreateInventory(size, slotCont, parent, ECS::e_AnchorSettings::Top, 0);
    m_inventory[group].emplace_back(newInv);
}

void InventoryManager::GroupLoadOrUnload(std::string group)
{



}

void InventoryManager::LoadInventory(GroupName group, std::string name)
{
    //create Group
   for (auto In : m_inventory[group]) {
        if (!In->GetIsDisplay() && In->GetName()== name) {
                In->LoadInventory(&EntityIDPannlSlot,false);
          }
   }
 
}

void InventoryManager::LoadInventoryGroup(std::string group)
{
        //create Group
        int index = 0;
        //find free buttion
        for (auto buttion: EntityIDButtion)
        {
            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText ==  "null") {
                break;
            }
            if (index == EntityIDButtion.size()) {
                //create new buttion
                ECS::UIButton* button = new ECS::UIButton();
                button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
                button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
                button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
                button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
                button->SetParent(ParentID);
                button->GetSpriteComponent()->toDraw = false;
                button->AddComponent<ECS::UIContainerComponent>();
                button->GetButtonComponent()->buttonText = "null";
                EntityIDButtion.push_back(button->GetEntityID());
                break;
            }
            index++;
        }

        if (EntityIDButtion.size() == 0) {
            ECS::UIButton* button = new ECS::UIButton();
            button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
            button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
            button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
            button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
            button->SetParent(ParentID);
            button->GetSpriteComponent()->toDraw = false;
            button->AddComponent<ECS::UIContainerComponent>();
            button->GetButtonComponent()->buttonText = "null";
            EntityIDButtion.push_back(button->GetEntityID());
        }

        int groupIndex=0;
        for (auto* In : m_inventory[group]) {
            if (!In->GetIsDisplay()) {

                //buttion creation / des
                if (!ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(In->GetEntityData())->isDisplayAll) {
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(EntityIDButtion[index])->buttonText = In->GetName();
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(EntityIDButtion[index])->toDraw = true;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index])->anchorSettings = In->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->anchorSettings;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index])->offSet= Maths::Vec2f( -In->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->defaultDimensions.x, 120.0f* groupIndex);
                //ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index]);
                    for (size_t i = 0; i < m_inventory[group].size(); i++)
                    {
                        if (i == groupIndex) {
                            continue;
                        }
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::UnloadInventory, m_inventory[group][i]));
                    }
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::LoadInventory, In, &EntityIDPannlSlot, false));

            //find next free buttion
                            index = 0;
                            for (auto buttion : EntityIDButtion)
                            {
                                if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText == "null") {
                                    break;
                                }
                                if (index == EntityIDButtion.size()-1) {
                                    //create new buttion
                                    index++;
                                    ECS::UIButton* button = new ECS::UIButton();
                                    button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
                                    button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
                                    button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
                                    button->SetOffset(Maths::Vec2f(0.0f, 150.0f*index));
                                    button->SetParent(ParentID);
                                    button->GetSpriteComponent()->toDraw = false;
                                    button->AddComponent<ECS::UIContainerComponent>();
                                    button->GetButtonComponent()->buttonText = "null";
                                    EntityIDButtion.push_back(button->GetEntityID());
                        
                                    break;
                                }
                                index++;
                            }

                }



                if (ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(In->GetEntityData())->isDisplayAll) {
                    In->LoadInventory(&EntityIDPannlSlot, false);
                }
                else if (groupIndex == 0) {
                    In->LoadInventory(&EntityIDPannlSlot, false);
                }

                

                groupIndex++;
            }
        }

        Events::EventDispatcher::InvokeListeners<Events::Inventory::LoadInventoryGroup>((void*)&group);
        Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

}

void InventoryManager::UnloadInventory(GroupName group, std::string name)
{
    //unloade Group
    for (auto In : m_inventory[group]) {
        if (In->GetIsDisplay() && In->GetName() == name) {
            In->UnloadInventory();
        }
    }
}

void InventoryManager::UnloadInventoryGroup(std::string group)
{
      //unloade Group
    int index = 0;
      for (auto In : m_inventory[group]) {
          for (auto buttion : EntityIDButtion)
          {
              if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText== In->GetName()) {
                  ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText = "null";
                  ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(buttion)->onLeftPressFunctions.clear();
                  ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(buttion)->toDraw = false;
              }
          }
          In->UnloadInventory();
          index++;
      }

      Events::EventDispatcher::InvokeListeners<Events::Inventory::UnloadInventoryGroup>((void*)&group);
}



//item data controll
void InventoryManager::AddItem(GroupName group,std::string name, Firelight::ECS::Entity* item)
{
    //Add Item
    for (auto In : m_inventory[group]) {
        if (In->GetName() == name) {
            In->AddItem(item);
            break;
        }
    }
}

void InventoryManager::RemoveItem(GroupName group, std::string name, Firelight::ECS::Entity* item)
{
    for (auto In : m_inventory[group]) {
        if (In->GetName() == name) {
            In->RemoveItem(item);
            break;
        }
    }
}

void InventoryManager::RemoveItem(GroupName group, std::string name, int item, int howMany)
{
    for (auto In : m_inventory[group]) {
        if (In->GetName() == name) {
            In->RemoveItemType(howMany, item);
            break;
        }
    }
}

std::vector<ECS::EntityID> InventoryManager::GetItems(GroupName group, std::string name, int item, int howMany)
{
    for (auto In : m_inventory[group]) {
        if (In->GetName() == name) {
            return In->GetItemType(howMany, item); 
        }
    }
}

bool InventoryManager::CheckInventory(ECS::EntityID ID, std::string InvName, GroupName group)
{
    bool isThere = false;
    for (auto In : m_inventory[group]) {
        if (In->GetName() == InvName) {
            isThere = In->FindItem(ID);
            break;
        }
    }
    return isThere;
}
