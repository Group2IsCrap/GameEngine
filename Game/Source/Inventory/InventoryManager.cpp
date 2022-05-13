#include "InventoryManager.h"
#include"InventoryComponents.h"
#include"Source/ECS/ECSEvents.h"
#include"Source/Events/Event.h"
#include "Source/Engine.h"

#include"InventoryFunctionsGlobal.h"


namespace InventorySystem {
    InventoryManager::InventoryManager()
    {
        //AddWhitelistComponent<InventoryComponent>();
        AddWhitelistComponent<InventoryComponentGroupID>();

        //sub to this event
        Events::EventDispatcher::SubscribeFunction<Events::Inv::UPDATEINV>(std::bind(&InventoryManager::ItemChangeInventory, this));
        Events::EventDispatcher::SubscribeFunction<Events::Inv::ADD_NEW_INV>(std::bind(&InventoryManager::CreateInvetory, this));

       
        Events::EventDispatcher::AddListener<Events::Inv::REMOVE_ITEM_TYPE>(this);
        Events::EventDispatcher::AddListener<Events::Inv::GET_ITEM_TYPE>(this);
        Events::EventDispatcher::AddListener<Events::Inv::GET_ITEM_TYPE_NUMBER>(this);
        Events::EventDispatcher::AddListener<Events::Inv::ADD_ITEM>(this);

       
    }

    InventoryManager::~InventoryManager()
    {
    }

    void InventoryManager::HandleEvents(const char* event, void* data)
    {
      
        //item controll
        if (event == Events::Inv::ADD_ITEM::sm_descriptor)
        {
            Global_Functions::AddOrRemoveStruct* Data = (Global_Functions::AddOrRemoveStruct*)data;

            Data->returnData = AddItem(Data->groupName, Data->InventoryName, Data->itemID);
        }
        else if (event == Events::Inv::REMOVE_ITEM::sm_descriptor)
        {
            Global_Functions::AddOrRemoveStruct* Data = (Global_Functions::AddOrRemoveStruct*)data;

            Data->returnData = RemoveItem(Data->groupName, Data->InventoryName, Data->itemID);
        }
        else if (event == Events::Inv::REMOVE_ITEM_TYPE::sm_descriptor)
        {
            Global_Functions::RemoveItemTypeData* Data = (Global_Functions::RemoveItemTypeData*)data;

            Data->returnData = RemoveItem(Data->groupName, Data->InventoryName, Data->Type,Data->Number);
        }
        else if (event == Events::Inv::GET_ITEM_TYPE::sm_descriptor)
        {
            Global_Functions::GetItemTypeData* Data = (Global_Functions::GetItemTypeData*)data;

            Data->returnData = GetItems(Data->groupName, Data->InventoryName, Data->Type, Data->Number);
        }
        else if (event == Events::Inv::GET_ITEM_TYPE_NUMBER::sm_descriptor)
        {
            Global_Functions::ItemTotalData* Data = (Global_Functions::ItemTotalData*)data;

            Data->ReturnData = GetNumberOfItemInInvetory(Data->groupName, Data->InventoryName, Data->type);
        }
        else if (event == Events::Inv::GET_SPECIAL_SLOT::sm_descriptor)
        {
            Global_Functions::SpecilaItemSlotData* Data = (Global_Functions::SpecilaItemSlotData*)data;

            Data->ReturnData = GetSpecialSlot(Data->groupName, Data->InventoryName, Data->SlotName);
        }
        else if (event == Events::Inv::REMOVE_INV::sm_descriptor)
        {
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
            ECS::EntityID entityID = m_entities[i]->GetEntityID(); 
            InventoryComponentGroupID* inventoryGroupData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(entityID);
            if (!m_Inventory.contains(inventoryGroupData->Group)) {
                m_Inventory[inventoryGroupData->Group]; 
                if (m_Inventory[inventoryGroupData->Group].size() == 0 && inventoryGroupData->keyToAcvate != Keys::KEY_INVALID)
                {
                 
                    Events::EventDispatcher::SubscribeFunction(inventoryGroupData->Group.c_str(), std::bind(&InventoryManager::GroupLoadOrUnload, this, inventoryGroupData->Group));
                    Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(inventoryGroupData->Group.c_str(), inventoryGroupData->keyToAcvate, Firelight::KeyEventType::KeyPressSingle);
                
                }
            }
           
            std::vector<InventoryComponent*> inventoryData = ECS::EntityComponentSystem::Instance()->GetComponents<InventoryComponent>(entityID);
           
            for (size_t i = 0; i < inventoryData.size(); i++)
            {
                bool toAdd = true;
                for (size_t j = 0; j < m_Inventory[inventoryGroupData->Group].size(); j++) {
                    if (m_Inventory[inventoryGroupData->Group][j]->GetInvetorNumber() == i) {
                        toAdd = false;
                        break;
                    }
                }
                if (toAdd) {
                    CreateInventory(inventoryGroupData->Group, inventoryData[i]->Name, inventoryData[i]->Size, Maths::Vec2f(inventoryData[i]->ColoumCount, inventoryData[i]->RowCount), UIParentID, inventoryData[i]->offset, inventoryData[i]->AnchorSettings);
                    m_Inventory[inventoryGroupData->Group].back()->SetEntityData(m_entities.back()->GetEntityID(),i);
                }
            }

        }
        
    }

    void InventoryManager::RemoveInvetory(std::string groupName,std::string invName)
    {
        int index = 0;
        for (auto& inventoryData : m_Inventory[groupName]) {
            if (inventoryData->GetName() != invName) {
                index++;
                continue;
            }
                delete inventoryData;
                inventoryData = nullptr;
                m_Inventory[groupName].erase(m_Inventory[groupName].begin() + index);
                break;
        }


            //remove group
        if (m_Inventory[groupName].size() == 0) {
            //TODO Remove
            //Events::EventDispatcher::UnsubscribeFunction(b->Group.c_str());
            //Firelight::Engine::Instance().GetKeyBinder().unv(b->Group.c_str(), b->keyToAcvate, Firelight::KeyEventType::KeyPressSingle);
        }
        

    }

    void InventoryManager::ItemChangeInventory()
    {
        //change inventorys
        bool toDrop = true;
        //find unplaced slots in all invetorys
        for (auto inventoryGroup : m_Inventory) {
            for (auto inventory : inventoryGroup.second)
            {
                if (inventory->GetNullSlotData()->size() == 0) {
                    continue;
                }
                for (auto Slot : *inventory->GetNullSlotData()) {
                    //find
                    for (auto In2 : m_Inventory) {
                        for (auto inv2 : In2.second)
                        {
                            //move to new invetory
                            //via buttion text
                            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.UITexID)->parentID)) {
                                if (inv2->GetName() == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.UITexID)->parentID)->buttonText) {
                                    toDrop = inv2->AddItem(Slot, false);
                                    if (!toDrop) {
                                        inventory->GetNullSlotData()->clear();
                                        return;
                                    }
                                }
                            }
                            //via parent ids from ECS
                            else if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.UITexID)->parentID)->parentID == inv2->GetInventorySpace()->GetEntityID()) {
                                toDrop = inv2->AddItem(Slot, true);
                                if (!toDrop) {
                                    inventory->GetNullSlotData()->clear();
                                    return;
                                }
                                if (toDrop) {
                                    //drop code here
                                    InventoryComponentOutPut* data = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentOutPut>(inventory->GetEntityData(), inventory->GetInvetorNumber());

                                    if (data) {
                                        for (auto& out : data->OutputCommand) {
                                            out();
                                        }
                                    }
                                    inventory->GetNullSlotData()->clear();
                                    return;
                                }
                                
                            }
                           
                        }
                    }
                }
                

            }
        }



    }

    void InventoryManager::CreateInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
    {
        Inventory* newInventory = new Inventory(InvName);
        newInventory->CreateInventoryNoPannel(size, columnRows, parent, anc, offSet);
        m_Inventory[group].emplace_back(newInventory);
    }

    void InventoryManager::CreateInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc)
    {
        Inventory* newInventory = new Inventory(InvName);
        newInventory->CreateInventoryNoPannel(size, slotCont, parent, ECS::e_AnchorSettings::Top, 0);
        m_Inventory[group].emplace_back(newInventory);
    }

    void InventoryManager::LoadInventory(GroupName group, std::string name)
    {
        //create Group
        for (auto Inventory : m_Inventory[group]) {
            if (!Inventory->GetIsDisplay() && Inventory->GetName() == name) {
                Inventory->LoadInventory(&m_EntityIDPannlSlot, false);
            }
        }

    }

    void InventoryManager::LoadInventoryGroup(std::string group)
    {
        //create Group
        int index = 0;
        //find free buttion
        for (auto button : m_EntityIDButton)
        {
            if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(button)->buttonText == "null") {
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->parentID = UIParentID;
                ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->hasParent = true;
                break;
            }
            if (index == m_EntityIDButton.size()) {
                //create new buttion
                ECS::UIButton* button = new ECS::UIButton();
                button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
                button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
                button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
                button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
                button->SetParent(UIParentID);
                button->GetSpriteComponent()->toDraw = false;
                button->AddComponent<ECS::UIContainerComponent>();
                button->GetButtonComponent()->buttonText = "null";
                m_EntityIDButton.push_back(button->GetEntityID());
                break;
            }
            index++;
        }

        if (m_EntityIDButton.size() == 0) {
            ECS::UIButton* button = new ECS::UIButton();
            button->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
            button->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
            button->SetDefaultDimensions(Maths::Vec3f(100, 120, 0));
            button->SetOffset(Maths::Vec2f(0.0f, 0.0f));
            button->SetParent(UIParentID);
            button->GetSpriteComponent()->toDraw = false;
            button->AddComponent<ECS::UIContainerComponent>();
            button->GetButtonComponent()->buttonText = "null";
            m_EntityIDButton.push_back(button->GetEntityID());
        }

        int groupIndex = 0;
        for (auto* Inventory : m_Inventory[group]) {
            if (!Inventory->GetIsDisplay()) {

                //buttion creation / des
                if (!ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(Inventory->GetEntityData())->isDisplayAll) {
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(m_EntityIDButton[index])->buttonText = Inventory->GetName();
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(m_EntityIDButton[index])->toDraw = true;
                    
                    //set from invetory buttion componect 
                    InventoryComponentButtonLayout* buttionLayout = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentButtonLayout>(Inventory->GetEntityData());
                    if (buttionLayout && buttionLayout->ToUse) {
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->anchorSettings = buttionLayout->AnchorSettings;
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->offSet = buttionLayout->OffSet;
                    }
                    else
                    {
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->anchorSettings = Inventory->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->anchorSettings;
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->offSet = Maths::Vec2f(-Inventory->GetInventorySpace()->GetComponent<ECS::UIBaseWidgetComponent>()->defaultDimensions.x, 120.0f * groupIndex);
                    }
                    
                   
                    for (size_t i = 0; i < m_Inventory[group].size(); i++)
                    {
                        if (i == groupIndex) {
                            continue;
                        }
                        ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(m_EntityIDButton[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::UnloadInventory, m_Inventory[group][i]));
                    }
                    ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIPressableComponent>(m_EntityIDButton[index])->onLeftPressFunctions.push_back(std::bind(&Inventory::LoadInventory, Inventory, &m_EntityIDPannlSlot, false));

                    
                    //find next free buttion
                    index = 0;
                    for (auto buttion : m_EntityIDButton)
                    {
                        if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText == "null") {
                            ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->parentID = UIParentID;
                            ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(m_EntityIDButton[index])->hasParent = true;
                            break;
                        }
                        if (index == m_EntityIDButton.size() - 1) {
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
                            m_EntityIDButton.push_back(button->GetEntityID());

                            break;
                        }
                        index++;
                    }

                }



                if (ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentGroupID>(Inventory->GetEntityData())->isDisplayAll) {
                    Inventory->LoadInventory(&m_EntityIDPannlSlot, false);
                }
                else if (groupIndex == 0) {
                    Inventory->LoadInventory(&m_EntityIDPannlSlot, false);
                }



                groupIndex++;
            }
        }

        Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

    }

    void InventoryManager::UnloadInventory(GroupName group, std::string name)
    {
        //unloade Group
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetIsDisplay() && inventory->GetName() == name) {
                inventory->UnloadInventory();
            }
        }
    }

    void InventoryManager::UnloadInventoryGroup(std::string group)
    {
        //unloade Group
        int index = 0;
        for (auto inventory : m_Inventory[group]) {
            for (auto buttion : m_EntityIDButton)
            {
                if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIButtonComponent>(buttion)->buttonText == inventory->GetName()) {
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
    void InventoryManager::AddItem(GroupName group, std::string Name, Firelight::ECS::Entity* item)
    {
        //Add Item
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                inventory->AddItem(item);
                break;
            }
        }
    }

    bool InventoryManager::AddItem(GroupName group, std::string Name, Firelight::ECS::EntityID item)
    {
        //Add Item
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->AddItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string Name, Firelight::ECS::Entity* item)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->RemoveItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string Name, Firelight::ECS::EntityID item)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->RemoveItem(item);
                break;
            }
        }
    }

    bool InventoryManager::RemoveItem(GroupName group, std::string Name, int item, int howMany)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->RemoveItemType(howMany, item);
                break;
            }
        }
    }

    std::vector<ECS::EntityID> InventoryManager::GetItems(GroupName group, std::string Name, int item, int howMany)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->GetItemType(howMany, item);
            }
        }
    }

    bool InventoryManager::CheckInventory(ECS::EntityID ID, std::string InvName, GroupName Group)
    {
        
        for (auto inventory : m_Inventory[Group]) {
            if (inventory->GetName() == InvName) {
                return inventory->FindItem(ID);
                break;
            }
        }
        
    }

    int InventoryManager::GetNumberOfItemInInvetory(GroupName group, std::string Name, int item)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->GetItemTypeTotal(item);
            }
        }
    }

    ECS::EntityID InventoryManager::GetSpecialSlot(GroupName group, std::string Name, std::string slotName)
    {
        for (auto inventory : m_Inventory[group]) {
            if (inventory->GetName() == Name) {
                return inventory->GetSpecialSlot(slotName);
            }
        }
    }

}