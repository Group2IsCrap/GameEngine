#include "InventoryManager.h"
#include"InventoryComponents.h"
#include"Source/ECS/ECSEvents.h"
#include"Source/Events/Event.h"
#include "Source/Engine.h"
InventoryManager::InventoryManager()
{
    AddWhitelistComponent<InventoryComponent>();
    AddWhitelistComponent<InventoryComponentGroupID>();

    //sub to this event
    Events::EventDispatcher::SubscribeFunction<Events::Inv::UPDATEINV>(std::bind(&InventoryManager::ItemChangeInventory, this));
    Events::EventDispatcher::SubscribeFunction<Events::Inv::ADD_NEW_INV>(std::bind(&InventoryManager::CreateInvetory, this));

    //Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(std::bind(&InventoryManager::CreateInvetory, this));
    //Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityDestroyedEvent>(std::bind(&InventoryManager::RemoveInvetory, this));

    //Events::EventDispatcher::AddListener<Events::Inv::LOAD_INVENTORY_GROUP>(this);
    //Events::EventDispatcher::AddListener<Events::Inv::UNLOAD_INVENTORY_GROUP>(this);
}

InventoryManager::~InventoryManager()
{
}

void InventoryManager::HandleEvents(const char* event, void* data)
{
    if (event == Events::Inv::LOAD_INVENTORY_GROUP::sm_descriptor) {
        const char* ab = (const char*)data;
       
        LoadInventoryGroup(ab);
    }
    else if(event == Events::Inv::UNLOAD_INVENTORY_GROUP::sm_descriptor)
    {
        const char* ab = (const char*)data;
        UnloadInventoryGroup(ab);
    }

}
void InventoryManager::GroupLoadOrUnload(std::string group)
{
    if (!m_InventoryLoaded[group]) {
        LoadInventoryGroup(group);
        m_InventoryLoaded[group] = true;
    }
    else {
        UnloadInventoryGroup(group);
        m_InventoryLoaded[group] = false;
    }

}



void InventoryManager::CreateInvetory()
{
    for (size_t i = 0; i < m_entities.size(); i++)
    {
        ECS::EntityID ab = m_entities[i]->GetEntityID();
        InventoryComponent* a = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(ab);
        InventoryComponentGroupID* b = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(ab);
        bool toAdd = true;
        for (auto In : m_Inventory[b->Group]) {
            if ( In->GetName() == a->Name) {
                toAdd = false;
            }
        }

        if (toAdd) {
            CreatInventory(b->Group, a->Name, a->Size, Maths::Vec2f(a->ColoumCount, a->RowCount), ParentID, a->offset,a->AnchorSettings);
            m_Inventory[b->Group].back()->SetEntityData(m_entities.back()->GetEntityID());
        }
    }

    for (auto group: m_Inventory)
    {
        m_InventoryLoaded[group.first];
        Events::EventDispatcher::SubscribeFunction(group.first.c_str(),std::bind(&InventoryManager::GroupLoadOrUnload, this, group.first));
        Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(group.first.c_str(), Keys::KEY_E, Firelight::KeyEventType::KeyPressSingle);
    }
}

void InventoryManager::RemoveInvetory()
{



}

void InventoryManager::ItemChangeInventory()
{
    //cheage inventorys
    bool toDrop = true;
    //find unplaced slots in all invetorys
    for (auto In : m_Inventory) {
        for (auto inv: In.second)
        {
            if (inv->GetNullSlotData()->size() == 0) {
                continue;
            }
            for (auto Slot : *inv->GetNullSlotData()) {
                //find
                for (auto In2 : m_Inventory) {
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

void InventoryManager::CreatInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreatInventoryNoPannel(size, columnRows, parent, anc, offSet);
    m_Inventory[group].emplace_back(newInv);
}

void InventoryManager::CreatInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
{
    Inventory* newInv = new Inventory(InvName);
    newInv->CreatInventoryNoPannel(size, slotCont, parent, ECS::e_AnchorSettings::Top, 0);
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
        //find free buttion
        for (auto buttion: EntityIDButtion)
        {
            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText ==  "null") {
                break;
            }
            if (index == EntityIDButtion.size()) {
                //create new buttion
                ECS::UIButton* button = new ECS::UIButton();
                button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
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
            button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
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
        for (auto* In : m_Inventory[group]) {
            if (!In->GetIsDisplay()) {

                //buttion creation / des
                if (!ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(In->GetEntityData())->isDisplayAll) {
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(EntityIDButtion[index])->buttonText = In->GetName();
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(EntityIDButtion[index])->toDraw = true;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index])->anchorSettings = In->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->anchorSettings;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index])->offSet= Maths::Vec2f( -In->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->defaultDimensions.x, 120.0f* groupIndex);
                //ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(EntityIDButtion[index]);
                    for (size_t i = 0; i < m_Inventory[group].size(); i++)
                    {
                        if (i == groupIndex) {
                            continue;
                        }
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(EntityIDButtion[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::UnloadInventory, m_Inventory[group][i]));
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
                                    button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
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

        Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

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
}



//item data controll
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
    bool isThere = false;
    for (auto In : m_Inventory[Group]) {
        if (In->GetName() == InvName) {
            isThere = In->FindItem(ID);
            break;
        }
    }
    return isThere;
}
