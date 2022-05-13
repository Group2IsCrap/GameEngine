#include "Source/Engine.h"

#include "FModAudio.h"

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/CameraEntity.h>
#include <Source/Maths/Vec2.h>
#include <Source/Graphics/AssetManager.h>
#include <Source/ImGuiUI/ImGuiManager.h>

#include "Source/Systems/PlayerSystem.h"
#include "Source/Player/PlayerEntity.h"
#include "Source/UI/PlayerHealthUI.h"
#include "Source/UI/DeathMenu.h"
#include "Source/Items/ItemDatabase.h"
#include "Source/Core//WorldEntity.h"

#include "Source/Events/InputEvents.h"
#include "Source/Core/Layers.h"

#include"Source/Inventory/InventoryManager.h"
#include"Source/Inventory/InventoryWrapper.h"
#include"Source/Inventory/InventoryFunctionsGlobal.h"
using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::Events::InputEvents;
using namespace snowFallAudio::FModAudio;

void BindDefaultKeys()
{
	KeyBinder* keyBinder = &Engine::Instance().GetKeyBinder();
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_W);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_A);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_S);
	keyBinder->BindKeyboardActionEvent(OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_D);
	keyBinder->BindControllerAxisEvent(OnPlayerMoveEvent::sm_descriptor, ControllerThumbsticks::LEFT);
	keyBinder->BindKeyboardActionEvent(RemoveHealthEvent::sm_descriptor, Keys::KEY_T, KeyEventType::KeyPressSingle);

	keyBinder->BindKeyboardActionEvent(OnInteractEvent::sm_descriptor, Keys::KEY_F, KeyEventType::KeyPressSingle);
	keyBinder->BindKeyboardActionEvent(SpawnItemEvent::sm_descriptor, Keys::KEY_E, KeyEventType::KeyPressSingle);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "Game Name", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// Register Systems
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<PlayerSystem>();
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<InventorySystem::InventoryManager>();
		// Register KeyBindings
		BindDefaultKeys();

		// Camera
		CameraEntity* camera = new CameraEntity();
		Engine::Instance().SetActiveCamera(camera);

		// Player
		PlayerEntity* player = new PlayerEntity();

		// World
		WorldEntity* world = new WorldEntity();


		UICanvas* canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0), static_cast<int>(RenderLayer::UI));
		PlayerHealthUI* playerHealthUI = new PlayerHealthUI(canvas, player->GetHealthComponent()->maxHealth);
		DeathMenu* deathMenu = new DeathMenu(canvas);

		InventorySystem::UIParentID = canvas->GetEntityID();
		InventoryWrapper* inv1 = new InventoryWrapper("PlayerInv", false, true, Keys::KEY_F);
		inv1->AddInventory("MainIven", 10, 3, Maths::Vec2f(300, 720 / 2), Maths::Vec2f(0, 0), ECS::e_AnchorSettings::TopRight);
		inv1->AddInventory("equaitment", 10, 3, Maths::Vec2f(300, 720 / 2), Maths::Vec2f(0, (720 / 2) + 100), ECS::e_AnchorSettings::TopRight);

		//InventoryWrapper* inv2 = new InventoryWrapper();
		InventoryWrapper* inv2 = new InventoryWrapper("PlayerInv2", false, true, Keys::KEY_J);
		inv2->AddInventory("MainIven2", 10, 3, Maths::Vec2f(300, 720 / 2), Maths::Vec2f(0, 0), ECS::e_AnchorSettings::TopLeft);
		inv2->AddInventory("equaitment2", 10, 3, Maths::Vec2f(300, 720 / 2), Maths::Vec2f(0, (720 / 2) + 100), ECS::e_AnchorSettings::TopLeft);
		// Load All Items
		ItemDatabase::Instance()->LoadItems("Assets/items.csv");
		InventorySystem::Global_Functions::AddItem("PlayerInv","equaitment", ItemDatabase::Instance()->CreateInstanceOfItem(1)->GetEntityID());

		while (Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			Maths::Vec3f desiredPosition = player->GetTransformComponent()->position;
			Maths::Vec3f smoothPosition = Maths::Vec3f::Lerp(camera->GetTransformComponent()->position, desiredPosition, 5 * Firelight::Engine::Instance().GetTime().GetDeltaTime());
			camera->GetTransformComponent()->position = smoothPosition;

			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}

		Serialiser::SaveSceneJSON();
	}

	return 0;
}
