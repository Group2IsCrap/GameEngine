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
#include "Source/UI/MainMenuUI.h"
#include "Source/UI/DeathMenu.h"
#include "Source/Items/ItemDatabase.h"
#include "Source/CoreComponents/AIComponent.h"
#include "Source/Core//WorldEntity.h"

#include "Source/ImGuiDebugLayer.h"
#include "Source/Events/InputEvents.h"
#include "Source/Core/Layers.h"
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>
#include "Source/Core/AIEntity.h"
#include "Source/AI/Enemies/AICrocodileEntity.h"
#include "Source/AI/Enemies/AIDeerEntity.h"
#include "Source/AI/AIBehaviourComponent.h"
#include "Source/WorldEntities/TreeEntity.h"

#include "Source/Inventory/InventoryEntity.h"
#include "Source/Inventory/InventoryManager.h"
#include "Source/Inventory/InventoryFunctionsGlobal.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::Events::InputEvents;
using namespace snowFallAudio::FModAudio;

void BindDefaultKeys()
{
	KeyBinder* keyBinder = &Engine::Instance().GetKeyBinder();
	keyBinder->BindKeyboardActionEvent(AttackEvent::sm_descriptor, Keys::KEY_E, KeyEventType::KeyPressSingle);

	keyBinder->BindKeyboardActionEvent(OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_W);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_A);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_S);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_D);

	keyBinder->BindKeyboardActionEvent(OnPlayerMoveUpEventRelease::sm_descriptor, Keys::KEY_W, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveLeftEventRelease::sm_descriptor, Keys::KEY_A, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveDownEventRelease::sm_descriptor, Keys::KEY_S, KeyEventType::KeyRelease);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveRightEventRelease::sm_descriptor, Keys::KEY_D, KeyEventType::KeyRelease);


	keyBinder->BindControllerAxisEvent(OnPlayerMoveEvent::sm_descriptor, ControllerThumbsticks::LEFT);

	keyBinder->BindKeyboardActionEvent(OnInteractEvent::sm_descriptor, Keys::KEY_I, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(SpawnItemEvent::sm_descriptor, Keys::KEY_M, KeyEventType::KeyPressSingle);
}

void SpawnItem0()
{
	//ItemDatabase::Instance()->CreateInstanceOfItem(0);
	ItemDatabase::Instance()->CreateInstanceOfItem(0)->GetEntityID();
}

void SpawnItem1()
{
	//ItemDatabase::Instance()->CreateInstanceOfItem(1);
	ItemDatabase::Instance()->CreateInstanceOfItem(3)->GetEntityID();
}

void SetupDebugUI()
{
	// ImGui Test code
	ImGuiDebugLayer* itemTestLayer = new ImGuiDebugLayer();
	itemTestLayer->spawnItemCommand[0] = std::bind(SpawnItem0);
	itemTestLayer->spawnItemCommand[1] = std::bind(SpawnItem1);
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(itemTestLayer);
}

void SetupEnemyTemplate()
{
	SpriteEntityTemplate* enemyTemplate = new SpriteEntityTemplate();
	AIComponent* aiComponent = enemyTemplate->AddComponent<AIComponent>();
	enemyTemplate->GetComponent<LayerComponent>()->layer = static_cast<int>(GameLayer::Enemy);
	SpriteComponent* spriteComponent = enemyTemplate->GetComponent<SpriteComponent>();
	spriteComponent->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/ShitDeer.png");
	spriteComponent->pixelsPerUnit = 50;
	spriteComponent->layer = static_cast<int>(RenderLayer::Enemy);
	enemyTemplate->AddComponent<RigidBodyComponent>();
	enemyTemplate->AddComponent<AIBehaviourComponent>();
	enemyTemplate->AddComponent<HealthComponent>();

	AIDeerEntity* entity1 = new AIDeerEntity(true, enemyTemplate->GetTemplateID());
	AICrocodileEntity* entity2 = new AICrocodileEntity(true, enemyTemplate->GetTemplateID());
}

void SetupResourceTemplate()
{
	SpriteEntityTemplate* resourceTemplate = new SpriteEntityTemplate();
	resourceTemplate->GetComponent<LayerComponent>()->layer = static_cast<int>(GameLayer::Resource);
	SpriteComponent* spriteComponent = resourceTemplate->GetComponent<SpriteComponent>();
	spriteComponent->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/ObjectSprites/Tree.png");
	spriteComponent->pixelsPerUnit = 50;
	spriteComponent->layer = static_cast<int>(RenderLayer::Items);
	resourceTemplate->AddComponent<RigidBodyComponent>();
	resourceTemplate->AddComponent<HealthComponent>();

	TreeEntity* entity1 = new TreeEntity(true, resourceTemplate->GetTemplateID());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "Don't Perish", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// Register Systems
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<PlayerSystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<AISystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<InventorySystem::InventoryManager>();

		// Register KeyBindings
		BindDefaultKeys();

		// Camera
		CameraEntity* camera = new CameraEntity();
		Engine::Instance().SetActiveCamera(camera);

		// Player
		PlayerEntity* player = new PlayerEntity();

		//AI
		SetupEnemyTemplate();
		SetupResourceTemplate();

		// Grass
		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
		test2->GetSpriteComponent()->pixelsPerUnit = 20.0f;
		test2->GetSpriteComponent()->layer = 16;
		// World
		WorldEntity* world = new WorldEntity();

		SpriteEntity* barn = new SpriteEntity();
		barn->GetComponent<TransformComponent>()->position.x = 10.0f;
		barn->GetComponent<TransformComponent>()->position.y = 10.0f;
		barn->GetComponent<SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/barn.png");
		barn->GetComponent<SpriteComponent>()->pixelsPerUnit = 50;
		barn->GetComponent<SpriteComponent>()->layer = 33;
		barn->AddComponent<RigidBodyComponent>();
		barn->GetComponent<StaticComponent>()->isStatic = true;
		BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(barn->AddComponent<ColliderComponent>(new BoxColliderComponent()));
		boxCollider->rect = Firelight::Maths::Rectf(-0.2f, -1.0f, 6.4f, 5.0f);
		boxCollider->drawCollider = true;
		//CircleColliderComponent* collider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(barn->AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::CircleColliderComponent()));
		//collider->drawCollider = true;
		//collider->radius = 4.25f;

		// UI
		UICanvas* canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0), static_cast<int>(RenderLayer::UI));
		PlayerHealthUI* playerHealthUI = new PlayerHealthUI(canvas, player->GetHealthComponent()->maxHealth);
		//MainMenuUI* mainMenuUI = new MainMenuUI(canvas);
		DeathMenu* deathMenu = new DeathMenu(canvas);

		// Debug UI
		SetupDebugUI();

		// Load All Items
		InventorySystem::UIParentID = canvas->GetEntityID();
		InventoryEntity* inv1 = new InventoryEntity("PlayerInventory", false, true, Keys::KEY_B);
		inv1->AddInventory("MainIven", 10, 3, Maths::Vec2f(300, 1080 / 2), Maths::Vec2f(0, (1080 / 2)), ECS::e_AnchorSettings::TopRight);
		inv1->AddInventory("Equipment", 8, 1, Maths::Vec2f(300, 1080 / 2), Maths::Vec2f(0, 0), ECS::e_AnchorSettings::TopRight);
		inv1->AddSpecialSlot(1, "Weapon", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::TopRight, std::vector<std::string>{ "Weapon" });
		inv1->AddSpecialSlot(1, "Head", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::TopLeft, std::vector<std::string>{ "Head" });
		inv1->AddSpecialSlot(1, "Body", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::Top, std::vector<std::string>{ "Chest" });
		inv1->AddSpecialSlot(1, "legs", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::Left, std::vector<std::string>{ "Legs" });
		inv1->AddSpecialSlot(1, "feet", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::Right, std::vector<std::string>{ "Back" });
		inv1->AddSpecialSlot(1, "a", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::BottomLeft, std::vector<std::string>{ "a" });
		inv1->AddSpecialSlot(1, "b", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::Bottom, std::vector<std::string>{ "b" });
		inv1->AddSpecialSlot(1, "c", Maths::Vec2f(0, 0), Maths::Vec2f(100, 100), ECS::e_AnchorSettings::BottomRight, std::vector<std::string>{ "c" });
		// Load All Items
		ItemDatabase::Instance()->LoadItems("Assets/items.csv");
		

		while (Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			Maths::Vec3f desiredPosition = player->GetTransformComponent()->position;
			Maths::Vec3f smoothPosition = Maths::Vec3f::Lerp(camera->GetTransformComponent()->position, desiredPosition, 5 * Firelight::Engine::Instance().GetTime().GetDeltaTime());
			camera->GetTransformComponent()->position = smoothPosition;

			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}

		//Serialiser::SaveSceneJSON();
	}

	return 0;
}
