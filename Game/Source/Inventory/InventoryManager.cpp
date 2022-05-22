#include "InventoryManager.h"
#include"InventoryComponents.h"
#include"Source/ECS/ECSEvents.h"
#include"Source/Events/Event.h"
#include "Source/Engine.h"

#include"InventoryFunctionsGlobal.h"


namespace InventorySystem 
{
    InventoryManager::InventoryManager()
    {
        //AddWhitelistComponent<InventoryComponent>();
        AddWhitelistComponent<InventoryComponentGroupID>();

        //sub to this event
        Events::EventDispatcher::SubscribeFunction<Events::Inventory::UpdateInventory>(std::bind(&InventoryManager::ItemChangeInventory, this));
        Events::EventDispatcher::SubscribeFunction<Events::Inventory::AddNewInventory>(std::bind(&InventoryManager::CreateNewInventory, this));

       
        Events::EventDispatcher::AddListener<Events::Inventory::RemoveItemType>(this);
        Events::EventDispatcher::AddListener<Events::Inventory::GetItemType>(this);
        Events::EventDispatcher::AddListener<Events::Inventory::GetItemTypeNumber>(this);
        Events::EventDispatcher::AddListener<Events::Inventory::AddItem>(this);

        ECS::UIPanel* Slot;
        for (size_t i = 0; i < 100; i++)
        {
            Slot = new ECS::UIPanel("Inventory Slot");
            Slot->GetWidgetComponent()->hasParent = false;
            Slot->GetSpriteComponent()->toDraw = false;
            Slot->GetWidgetComponent()->isActive = false;
           
            m_entityIDPanelSlot.push_back(Slot);
        }
    }

    InventoryManager::~InventoryManager()
    {
    }

    void InventoryManager::HandleEvents(const char* event, void* eventData)
    {
      
        //item control
        if (event == Events::Inventory::AddItem::sm_descriptor)
        {
            GlobalFunctions::AddOrRemoveStruct* data = (GlobalFunctions::AddOrRemoveStruct*)eventData;

            data->returnData = AddItem(data->groupName, data->inventoryName, data->itemID);
        }
        else if (event == Events::Inventory::RemoveItem::sm_descriptor)
        {
            GlobalFunctions::AddOrRemoveStruct* data = (GlobalFunctions::AddOrRemoveStruct*)eventData;

            data->returnData = RemoveItem(data->groupName, data->inventoryName, data->itemID);
        }
        else if (event == Events::Inventory::RemoveItemType::sm_descriptor)
        {
            GlobalFunctions::RemoveItemTypeData* data = (GlobalFunctions::RemoveItemTypeData*)eventData;

            data->returnData = RemoveItem(data->groupName, data->inventoryName, data->Type,data->Number);
        }
        else if (event == Events::Inventory::GetItemType::sm_descriptor)
        {
            GlobalFunctions::GetItemTypeData* data = (GlobalFunctions::GetItemTypeData*)eventData;

            data->returnData = GetItems(data->groupName, data->inventoryName, data->Type, data->Number);
        }
        else if (event == Events::Inventory::GetItemTypeNumber::sm_descriptor)
        {
            GlobalFunctions::ItemTotalData* data = (GlobalFunctions::ItemTotalData*)eventData;

            data->returnData = GetNumberOfItemInInventory(data->groupName, data->inventoryName, data->type);
        }
        else if (event == Events::Inventory::GetSpecialSlot::sm_descriptor)
        {
            GlobalFunctions::SpecialItemSlotData* data = (GlobalFunctions::SpecialItemSlotData*)eventData;

            data->returnData = GetSpecialSlot(data->groupName, data->inventoryName, data->slotName);
        }
        else if (event == Events::Inventory::RemoveInventory::sm_descriptor)
        {
            GlobalFunctions::RemoveInvetoryData* data = (GlobalFunctions::RemoveInvetoryData*)eventData;
            RemoveInventory(data->groupName, data->inventoryName);
        }
    }
    void InventoryManager::GroupLoadOrUnload(std::string group)
    {
        if (!m_inventoryLoaded[group]) 
        {
            LoadInventoryGroup(group);
            m_inventoryLoaded[group] = true;
        }
        else 
        {
            UnloadInventoryGroup(group);
            m_inventoryLoaded[group] = false;
        }

    }

    void InventoryManager::CreateNewInventory()
    {
        for (size_t i = 0; i < m_entities.size(); i++)
        {
            ECS::EntityID entityID = m_entities[i]->GetEntityID(); 
            InventoryComponentGroupID* inventoryGroupData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(entityID);
            if (!m_inventory.contains(inventoryGroupData->group)) 
            {
                m_inventory[inventoryGroupData->group]; 
                if (m_inventory[inventoryGroupData->group].size() == 0 && inventoryGroupData->keyToActivate != Keys::KEY_INVALID)
                {
                 
                    m_inventoryEventsIDs[inventoryGroupData->group]=Events::EventDispatcher::SubscribeFunction(inventoryGroupData->group.c_str(), std::bind(&InventoryManager::GroupLoadOrUnload, this, inventoryGroupData->group));
                    Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(inventoryGroupData->group.c_str(), inventoryGroupData->keyToActivate, Firelight::KeyEventType::KeyPressSingle);
                
                }
            }
           
            std::vector<InventoryComponent*> inventoryData = ECS::EntityComponentSystem::Instance()->GetComponents<InventoryComponent>(entityID);
           
            for (size_t i = 0; i < inventoryData.size(); i++)
            {
                bool toAdd = true;
                for (size_t j = 0; j < m_inventory[inventoryGroupData->group].size(); j++) 
                {
                    if (m_inventory[inventoryGroupData->group][j]->GetInventoryNumber() == i) 
                    {
                        toAdd = false;
                        break;
                    }
                }
                if (toAdd) 
                {
                    CreateInventory(inventoryGroupData->group, inventoryData[i]->name, inventoryData[i]->size, Maths::Vec2f(inventoryData[i]->columnCount, inventoryData[i]->rowCount), inventoryData[i]->margin, UIParentID, inventoryData[i]->offset, inventoryData[i]->anchorSettings);
                    m_inventory[inventoryGroupData->group].back()->SetEntityData(m_entities.back()->GetEntityID(),i);
                }
            }

        }
        
    }

    void InventoryManager::RemoveInventory(std::string groupName,std::string inventoryName)
    {
        int index = 0;
        for (auto& inventoryData : m_inventory[groupName]) 
        {
            if (inventoryData->GetName() != inventoryName) 
            {
                index++;
                continue;
            }

            delete inventoryData;
            inventoryData = nullptr;
            m_inventory[groupName].erase(m_inventory[groupName].begin() + index);
            break;
        }


        //remove group
        if (m_inventory[groupName].size() == 0) 
        {
            //TODO Remove
            Events::EventDispatcher::UnsubscribeFunction(groupName.c_str(),m_inventoryEventsIDs[groupName]);
            //Firelight::Engine::Instance().GetKeyBinder().unv(b->Group.c_str(), b->keyToAcvate, Firelight::KeyEventType::KeyPressSingle);
        }
        

    }

    void InventoryManager::ItemChangeInventory()
    {
        //change inventorys
        bool toDrop = true;
        //find unplaced slots in all invetorys
        for (auto& inventoryGroup : m_inventory) 
        {
            for (auto& inventory : inventoryGroup.second)
            {
                if (inventory->GetNullSlotData()->size() == 0)
                {
                    continue;
                }
                for (auto& slot : *inventory->GetNullSlotData()) 
                {
                    //find
                    for (auto& inventoryGroup2 : m_inventory)
                    {
                        for (auto& inventory2 : inventoryGroup2.second)
                        {
                            if (inventory2->GetName() == inventory->GetName()) {
                                continue;
                            }
                            //move to new invetory
                            //via buttion text
                            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot.UITexID)->parentID)) 
                            {
                                if (inventory2->GetName() == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot.UITexID)->parentID)->buttonText) 
                                {
                                    toDrop = inventory2->AddItem(slot, false);
                                    if (!toDrop) {
                                        inventory->GetNullSlotData()->clear();
                                        return;
                                    }
                                }
                            }
                            //via parent ids from ECS
                            else if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot.UITexID)->parentID)->parentID == inventory2->GetInventorySpace()->GetEntityID()) 
                            {
                                toDrop = inventory2->AddItem(slot, true);
                                if (!toDrop) 
                                {
                                    inventory->GetNullSlotData()->clear();
                                    return;
                                }if (toDrop)
                                {
                                    //drop code here
                                    InventoryComponentOutPut* data = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentOutPut>(inventory2->GetEntityData(), inventory2->GetInventoryNumber());

                                if (data) {
                                    InventoryStoreData a = inventory->GetNullSlotData()->front();
                                    for (auto& out : data->outputCommand)
                                    {
                                        out((void*)&a.entityIDs);
                                    }
                                }
                                inventory->GetNullSlotData()->clear();
                                return;
                                }
                            }
                            
                        }
                       
                    }


                }
                 if (toDrop)
                 {
                    //drop code here
                        InventoryComponentOutPut* data = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentOutPut>(inventory->GetEntityData(), inventory->GetInventoryNumber());

                    if (data) {
                        InventoryStoreData a = inventory->GetNullSlotData()->front();
                        for (auto& out : data->outputCommand)
                        {
                            out((void*)&a.entityIDs);
                        }
                    }
                    inventory->GetNullSlotData()->clear();
                    //return;
                }

            }
        }
       


    }

    void InventoryManager::CreateInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, Maths::Vec2f margin, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
    {
        Inventory* newInventory = new Inventory(InvName);
        newInventory->CreateInventory(size, columnRows, margin, parent, anc, offSet);
        m_inventory[group].emplace_back(newInventory);
    }

    void InventoryManager::CreateInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, Maths::Vec2f margin, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
    {
        Inventory* newInventory = new Inventory(InvName);
        newInventory->CreateInventory(size, slotCont, margin, parent, ECS::e_AnchorSettings::Top, 0);
        m_inventory[group].emplace_back(newInventory);
    }

    void InventoryManager::LoadInventory(GroupName group, std::string name)
    {
        //create Group
        for (auto Inventory : m_inventory[group]) {
            if (!Inventory->GetIsDisplay() && Inventory->GetName() == name) {
                Inventory->LoadInventory(&m_entityIDPanelSlot, false);
            }
        }

    }

    void InventoryManager::LoadInventoryGroup(std::string group)
    {
        //create Group
        int index = 0;
        //find free buttion
        for (auto button : m_entityIDButton)
        {
            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(button)->buttonText == "null") 
            {
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->parentID = UIParentID;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->hasParent = true;
                break;
            }
            if (index == m_entityIDButton.size()) 
            {
                //create new buttion
                ECS::UIButton* button = new ECS::UIButton("Load Inventory Group Button");
                button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
                button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
                button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
                button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
                button->SetParent(UIParentID);
                button->GetSpriteComponent()->toDraw = false;
                button->AddComponent<ECS::UIContainerComponent>();
                button->GetButtonComponent()->buttonText = "null";
                m_entityIDButton.push_back(button->GetEntityID());
                break;
            }
            index++;
        }

        if (m_entityIDButton.size() == 0) 
        {
            ECS::UIButton* button = new ECS::UIButton("Load Inventory Group Button");
            button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
            button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
            button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
            button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
            button->SetParent(UIParentID);
            button->GetSpriteComponent()->toDraw = false;
            button->AddComponent<ECS::UIContainerComponent>();
            button->GetButtonComponent()->buttonText = "null";
            m_entityIDButton.push_back(button->GetEntityID());
        }

        int groupIndex = 0;
        for (auto* Inventory : m_inventory[group]) 
        {
            if (!Inventory->GetIsDisplay())
            {

                //buttion creation / des
                if (!ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(Inventory->GetEntityData())->isDisplayAll) 
                {
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(m_entityIDButton[index])->buttonText = Inventory->GetName();
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(m_entityIDButton[index])->toDraw = true;
                    
                    //set from invetory buttion componect 
                    InventoryComponentButtonLayout* buttionLayout = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentButtonLayout>(Inventory->GetEntityData());
                    if (buttionLayout && buttionLayout->toUse) 
                    {
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->anchorSettings = buttionLayout->anchorSettings;
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->offSet = buttionLayout->offset;
                    }
                    else
                    {
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->anchorSettings = Inventory->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->anchorSettings;
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->offSet = Maths::Vec2f(-Inventory->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->defaultDimensions.x, 120.0f * groupIndex);
                    }
                    
                   
                    for (size_t i = 0; i < m_inventory[group].size(); i++)
                    {
                        if (i == groupIndex) 
                        {
                            continue;
                        }
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(m_entityIDButton[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::UnloadInventory, m_inventory[group][i]));
                    }
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(m_entityIDButton[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::LoadInventory, Inventory, &m_entityIDPanelSlot, false));

                    
                    //find next free buttion
                    index = 0;
                    for (auto buttion : m_entityIDButton)
                    {
                        if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText == "null") 
                        {
                            ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->parentID = UIParentID;
                            ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_entityIDButton[index])->hasParent = true;
                            break;
                        }
                        if (index == m_entityIDButton.size() - 1) 
                        {
                            //create new buttion
                            index++;
                            ECS::UIButton* button = new ECS::UIButton();
                            button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
                            button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
                            button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
                            button->SetOffset(Maths::Vec2f(0.0f, 150.0f * index));
                            button->SetParent(UIParentID);
                            button->GetSpriteComponent()->toDraw = false;
                            button->AddComponent<ECS::UIContainerComponent>();
                            button->GetButtonComponent()->buttonText = "null";
                            m_entityIDButton.push_back(button->GetEntityID());

                            break;
                        }
                        index++;
                    }

                }



                if (ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(Inventory->GetEntityData())->isDisplayAll) 
                {
                    Inventory->LoadInventory(&m_entityIDPanelSlot, false);
                }
                else if (groupIndex == 0) 
                {
                    Inventory->LoadInventory(&m_entityIDPanelSlot, false);
                }



                groupIndex++;
            }
        }

       
        Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
    }

    void InventoryManager::UnloadInventory(GroupName group, std::string name)
    {
        //unloade Group
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetIsDisplay() && inventory->GetName() == name)
            {
                inventory->UnloadInventory();
            }
        }
    }

    void InventoryManager::UnloadInventoryGroup(std::string group)
    {
        //unloade Group
        int index = 0;
        for (auto inventory : m_inventory[group])
        {
            for (auto buttion : m_entityIDButton)
            {
                if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText == inventory->GetName()) 
                {
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText = "null";
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(buttion)->onLeftPressFunctions.clear();
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(buttion)->toDraw = false;
                }
            }
            inventory->UnloadInventory();
            index++;
        }
    }



    //item data controll
    void InventoryManager::AddItem(GroupName group, std::string name, Firelight::ECS::Entity* item)
    {
        //Add Item
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                inventory->AddItem(item);
                break;
            }
        }
    }

    bool InventoryManager::AddItem(GroupName group, std::string name, Firelight::ECS::EntityID item)
    {
        //Add Item
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name)
            {
                return inventory->AddItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string name, Firelight::ECS::Entity* item)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->RemoveItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string name, Firelight::ECS::EntityID item)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->RemoveItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string name, int item, int howMany)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->RemoveItemType(howMany, item);
                break;
            }
        }
    }

    std::vector<ECS::EntityID> InventoryManager::GetItems(GroupName group, std::string name, int item, int howMany)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->GetItemType(howMany, item);
            }
        }
    }

    bool InventoryManager::CheckInventory(ECS::EntityID ID, std::string InvName, GroupName group)
    {
        
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == InvName) 
            {
                return inventory->FindItem(ID);
                break;
            }
        }
        
    }

    int InventoryManager::GetNumberOfItemInInventory(GroupName group, std::string name, int item)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->GetItemTypeTotal(item);
            }
        }
    }

    ECS::EntityID InventoryManager::GetSpecialSlot(GroupName group, std::string name, std::string slotName)
    {
        for (auto inventory : m_inventory[group]) 
        {
            if (inventory->GetName() == name) 
            {
                return inventory->GetSpecialSlot(slotName);
            }
        }
    }

}