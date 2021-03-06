#include "Source/Engine.h"

#include "FModAudio.h"

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/CameraEntity.h>
#include <Source/Maths/Vec2.h>
#include <Source/Graphics/AssetManager.h>
#include <Source/ImGuiUI/ImGuiManager.h>

#include "Source/Systems/PlayerSystem.h"
#include "Source/Systems/AISystem.h"
#include "Source/Player/PlayerEntity.h"
#include "Source/UI/PlayerHealthUI.h"
#include "Source/Player/PlayerComponent.h"
#include "Source/UI/MainMenuUI.h"
#include "Source/UI/DeathMenu.h"
#include "Source/Items/ItemDatabase.h"
#include "Source/Items/CraftingRecipeDatabase.h"
#include "Source/WorldEntities/ResourceDatabase.h"
#include "Source/CoreComponents/AIComponent.h"
#include "Source/Core//WorldEntity.h"

#include "Source/ImGuiDebugLayer.h"
#include "Source/Events/GameEvents.h"
#include "Source/Events/InputEvents.h"
#include "Source/Core/Layers.h"
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>
#include "Source/Core/AIEntity.h"
#include "Source/AI/Enemies/AICrocodileEntity.h"
#include "Source/AI/Enemies/AIDeerEntity.h"
#include "Source/AI/Enemies/AISlimeEntity.h"
#include "Source/AI/AIBehaviourComponent.h"


#include <Source/ECS/Components/AnimationComponent.h>
#include "Source/Inventory/InventoryEntity.h"
#include "Source/Inventory/InventorySystem.h"
#include "Source/Inventory/InventoryFunctionsGlobal.h"
#include "Source/WorldEntities/EntitySpawnerComponent.h"
#include "Source/WorldEntities/EntitySpawnerSystem.h"

#include "Source/WorldEntities/PortalEntity.h"

#include "Source/WorldEntities/BackgroundMusicEntity.h"

#include "Source/Events/PlayerEvents.h"


#include "Source/PCG/BiomeInfo.h"
#include "Source/PCG/BiomeGeneration.h"
#include "Source/PCG/EnvironmentGeneration.h"
#include "Source/TileMap/TileMap.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::Events::InputEvents;
using namespace snowFallAudio::FModAudio;

static UICanvas* canvas = nullptr;
static PlayerEntity* player = nullptr;
static PlayerHealthUI* playerHealthUI = nullptr;

static bool g_RenderDebug = false;
static ImGuiDebugLayer* g_debugLayer = new ImGuiDebugLayer();

Firelight::Maths::Vec3f g_cameraVelocityRef;

static void ToggleDebugLayer()
{
	g_RenderDebug = !g_RenderDebug;

	if (g_RenderDebug)
	{
		Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(g_debugLayer);
	}
	else
	{
		Firelight::ImGuiUI::ImGuiManager::Instance()->RemoveRenderLayer(g_debugLayer);
	}
}


void TogglePause()
{
	Engine::Instance().TogglePause();
}

void SpawnItem0()
{
	//ItemDatabase::Instance()->CreateInstanceOfItem(0);
	ItemDatabase::Instance()->CreateInstanceOfItem(31);
	ItemDatabase::Instance()->CreateInstanceOfItem(33);
	ItemDatabase::Instance()->CreateInstanceOfItem(43);
	
}

void SpawnItem1()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(0);
	ItemDatabase::Instance()->CreateInstanceOfItem(1);
	ItemDatabase::Instance()->CreateInstanceOfItem(2);
	ItemDatabase::Instance()->CreateInstanceOfItem(3);
	ItemDatabase::Instance()->CreateInstanceOfItem(31);
}

void GenerateBiomeUI()
{
	//BiomeGeneration::Instance()->Render();
}


void SetupDebugUI()
{
	// ImGui Test code
	g_debugLayer->Initialize();
	g_debugLayer->spawnItemCommand[0] = std::bind(SpawnItem0);
	g_debugLayer->spawnItemCommand[1] = std::bind(SpawnItem1);
}

void SetupEnemySpawner()
{
	GameEntity* crocodileSpawner = new GameEntity("Crocodile Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->enemyName = "Crocodile";
	spawnerComponent->respawnCooldown = 3;
	crocodileSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);

	crocodileSpawner->GetTransformComponent()->SetPosition(Vec3f(50.0f, 0.0f, 0.0f));
}

void SetupResourceSpawner()
{
	GameEntity* treeSpawner = new GameEntity("Tree Spawner");
	EntitySpawnerComponent* spawnerComponent = new EntitySpawnerComponent();
	spawnerComponent->resourceID = 0;
	spawnerComponent->respawnCooldown = 3;
	treeSpawner->AddComponent<EntitySpawnerComponent>(spawnerComponent);
}

void DropItemAt(Maths::Vec3f at, EntityID toDrop) 
{

	Maths::Vec3f atPos= at;
	ECS::TransformComponent* toDropData = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TransformComponent>(toDrop);
	if (toDropData) 
	{
		toDropData->SetPosition(Maths::Random::RandomPointInCircle(atPos, 3));
	}
}

void DropItemAtPlayer(void* toDrop, EntityID player)
{
	std::vector<EntityID> DropIDs= *(std::vector <EntityID>*)toDrop;
	ECS::TransformComponent* toDropData = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TransformComponent>(player);
	for (EntityID DropID : DropIDs) {
		DropItemAt(toDropData->GetPosition(), DropID);
	}
}

void ReAddToPlayer(void* toAdd) 
{
	std::vector<EntityID>* AddIDs = (std::vector <EntityID>*)toAdd;
	std::vector<EntityID> toRemove;
	for (size_t i = 0; i < AddIDs->size(); i++)
	{
		if (!InventorySystem::GlobalFunctions::AddItem("PlayerInventory", "MainInventory", AddIDs->at(i))) {
			toRemove.push_back(AddIDs->at(i));
		}
	}
	//fun remove loops 
	for (size_t i = 0; i < toRemove.size(); i++)
	{
		for (size_t j = 0; j < AddIDs->size(); j++)
		{
			if (AddIDs->at(j) == toRemove[i]) {
				AddIDs->erase(AddIDs->begin() + j);
				break;
			}
		}
	}
}

//void SetupResourceTemplate()
//{
//	ResourceEntity* treeEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(0);
//	treeEntity->GetTransformComponent()->SetPosition(Maths::Vec3f(5.0f, 5.0f, 0.0f));
//	ResourceEntity* rockEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(1);
//	rockEntity->GetTransformComponent()->SetPosition(Maths::Vec3f(-5.0f, 5.0f, 0.0f));
//	ResourceEntity* bushEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(2);
//	bushEntity->GetTransformComponent()->SetPosition(Maths::Vec3f(5.0f, -5.0f, 0.0f));
//	ResourceEntity* berryBushEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(3);
//	berryBushEntity->GetTransformComponent()->SetPosition(Maths::Vec3f(-5.0f, -5.0f, 0.0f));
//
//}



void PlayGame()
{
	if (playerHealthUI != nullptr && player != nullptr)
	{
		playerHealthUI->SetHealth(player->GetHealth());
	}
}


void BindDefaultKeys()
{
	KeyBinder* keyBinder = &Engine::Instance().GetKeyBinder();
	keyBinder->BindKeyboardActionEvent(AttackEvent::sm_descriptor, Keys::KEY_E, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(ReleaseAttackEvent::sm_descriptor, Keys::KEY_E, KeyEventType::KeyRelease);


	keyBinder->BindKeyboardActionEvent(OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_W, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_A, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_S, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_D, KeyEventType::KeyPressSingle);

	keyBinder->BindKeyboardActionEvent(OnPlayerMoveUpEventRelease::sm_descriptor, Keys::KEY_W, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveLeftEventRelease::sm_descriptor, Keys::KEY_A, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveDownEventRelease::sm_descriptor, Keys::KEY_S, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveRightEventRelease::sm_descriptor, Keys::KEY_D, KeyEventType::KeyRelease);

	keyBinder->BindKeyboardActionEvent(ShowDebugEvent::sm_descriptor, Keys::KEY_FUNCTION_2, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(TogglePauseEvent::sm_descriptor, Keys::KEY_ESCAPE, KeyEventType::KeyPressSingle);


	keyBinder->BindControllerAxisEvent(OnPlayerMoveEvent::sm_descriptor, ControllerThumbsticks::LEFT);

	keyBinder->BindKeyboardActionEvent(OnInteractEvent::sm_descriptor, Keys::KEY_I, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(SpawnItemEvent::sm_descriptor, Keys::KEY_M, KeyEventType::KeyPressSingle);

	Firelight::Events::EventDispatcher::SubscribeFunction<ShowDebugEvent>(std::bind(&ToggleDebugLayer));
	Firelight::Events::EventDispatcher::SubscribeFunction<PlayGameEvent>(std::bind(&PlayGame));
	Firelight::Events::EventDispatcher::SubscribeFunction<TogglePauseEvent>(std::bind(&TogglePause));
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "Firelight", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// Register Systems
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<InventorySystem::InventorySystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<PlayerSystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<AISystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<EntitySpawnerSystem>();

		// Register KeyBindings
		BindDefaultKeys();

		// Camera
		CameraEntity* camera = Engine::Instance().GetActiveCamera();

		// Player
		player = new PlayerEntity();

		//AI
		ResourceDatabase::Instance()->LoadResources("Assets/ResourceDatabase.csv");
		//SetupEnemySpawner();
		//SetupResourceSpawner();

		// Tilemap
		Firelight::TileMap::TileMap* tileMap = new Firelight::TileMap::TileMap();
		tileMap->SetBottomLeftTilePos(Firelight::Maths::Vec2f(-50.0f, -50.0f));
		tileMap->UpdateTileMapPositions();

		//Biome Generation
		BiomeInfo* biomeInfo = new BiomeInfo();
		BiomeGeneration::Instance()->Initialise(tileMap, biomeInfo);
		BiomeGeneration::Instance()->GenerateWorld();

		tileMap->Render();

		//Resource PCG spawning
		EnvironmentGeneration::Instance()->Initialise(tileMap, biomeInfo);
		EnvironmentGeneration::Instance()->GenerateResources();
		BiomeGeneration::Instance()->KillVoidTiles();

		// UI
		canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0), static_cast<int>(RenderLayer::UI));
		playerHealthUI = new PlayerHealthUI(canvas, player->GetHealthComponent()->maxHealth);
		MainMenuUI* mainMenuUI = new MainMenuUI(canvas);
		DeathMenu* deathMenu = new DeathMenu(canvas);

		// Debug UI
		SetupDebugUI();

		// Player Event
		Firelight::Events::EventDispatcher::InvokeFunctions(Firelight::Events::PlayerEvents::OnPlayerCreated::sm_descriptor, (void*)player->GetEntityID());

		// Load All Items
		InventorySystem::UIParentID = canvas->GetEntityID();
		InventoryEntity* inv1 = new InventoryEntity("PlayerInventory", false, true, Keys::KEY_B);
		inv1->AddInventory("MainInventory", "Sprites/UI/PlayerInventory.png", 1, 10, Maths::Vec2f(1000, 100), Maths::Vec2f(75, 75), Maths::Vec2f(0, 0), Maths::Vec2f(12.5f, 12.5f), Maths::Vec2f(-100, 0), ECS::e_AnchorSettings::Bottom);
		inv1->AddInventory("Equipment", "Sprites/UI/PlayerInventory1.png", 1, 3, Maths::Vec2f(300 - 12.5f, 100), Maths::Vec2f(0, 0), Maths::Vec2f(25, 0), Maths::Vec2f(75 * 4 + (25.0f * 3) - 12.5f + 50, 0), ECS::e_AnchorSettings::Bottom);

		inv1->AddSpecialSlot(1, "Weapon", "Sprites/UI/Slot_Weapon.png", Maths::Vec2f(-75 - 12.5f, 0), Maths::Vec2f(75, 75), ECS::e_AnchorSettings::Center, std::vector<std::string>{ "Weapon" });
		inv1->AddSpecialSlot(1, "Head", "Sprites/UI/Slot_Hat.png", Maths::Vec2f(0, 0), Maths::Vec2f(75, 75), ECS::e_AnchorSettings::Center, std::vector<std::string>{ "Helm" });
		inv1->AddSpecialSlot(1, "Body", "Sprites/UI/SlotIcon.png", Maths::Vec2f(75 + 12.5f, 0), Maths::Vec2f(75, 75), ECS::e_AnchorSettings::Center, std::vector<std::string>{ "Chest" });
		Keys key = Keys::KEY_1;
		for (int i = 0; i < 10; i++)
		{
			std::string ActionName = "Action" + std::to_string(i);
			inv1->AddKeyCommands(0, key, ActionName, std::to_string(i));
			key = (Keys)((int)key + 1);
			if (i == 8) {
				key= Keys::KEY_0;
			}
		}
		
		inv1->AddOutputCommands(0,std::bind(&DropItemAtPlayer,std::placeholders::_1, player->GetEntityID()));
		inv1->AddOutputCommands(1, std::bind(&ReAddToPlayer, std::placeholders::_1));
		inv1->AddOutputCommands(1, std::bind(&DropItemAtPlayer, std::placeholders::_1, player->GetEntityID()));

		// Load all items and recipes
		ItemDatabase::Instance()->LoadItems("Assets/items.csv");
		CraftingRecipeDatabase::Instance().LoadCraftingRecipes("Assets/crafting_recipes.csv");

		Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::PlayerEvents::ChangeWeapon>();

		// Portal
		PortalEntity* portalEntity = new PortalEntity();
		portalEntity->GetTransformComponent()->SetPosition(Vec3f(0.0f, 5.0f, 0.0f));

		BackgroundMusicEntity* backgroundMusic = new BackgroundMusicEntity();

		while (Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			Maths::Vec3f desiredPosition = player->GetTransformComponent()->GetPosition();
			TransformComponent* transformComponent = camera->GetTransformComponent();
			camera->GetTransformComponent()->SetPosition(
				Firelight::Maths::Vec3f::SmoothDamp(transformComponent->GetPosition(), desiredPosition, g_cameraVelocityRef, 0.35f, 10, Engine::Instance().GetTime().GetDeltaTime()));

			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}

