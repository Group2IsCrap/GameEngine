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
#include "Source/ImGuiDebugLayer.h"
#include "Source/Items/ItemDatabase.h"

#include "Source/Events/InputEvents.h"

#include"Source/Inventory/InventoryManager.h"
#include"Source/Inventory/InventoryWrapper.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace snowFallAudio::FModAudio;

void SpawnItem0()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(0);
}

void SpawnItem1()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(1);
}

void SetupDebugUI()
{
	// ImGui Test code
	ImGuiDebugLayer* itemTestLayer = new ImGuiDebugLayer();
	itemTestLayer->spawnItemCommand[0] = std::bind(SpawnItem0);
	itemTestLayer->spawnItemCommand[1] = std::bind(SpawnItem1);
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(itemTestLayer);
}

void BindDefaultKeys()
{
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_W);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_A);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_S);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_D);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_UP_ARROW);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_LEFT_ARROW);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_DOWN_ARROW);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_RIGHT_ARROW);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerActionEvent(Firelight::Events::InputEvents::OnPlayerMoveUpEvent::sm_descriptor, Events::Input::ControllerButtons::DPAD_UP);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerActionEvent(Firelight::Events::InputEvents::OnPlayerMoveLeftEvent::sm_descriptor, Events::Input::ControllerButtons::DPAD_LEFT);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerActionEvent(Firelight::Events::InputEvents::OnPlayerMoveDownEvent::sm_descriptor, Events::Input::ControllerButtons::DPAD_DOWN);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerActionEvent(Firelight::Events::InputEvents::OnPlayerMoveRightEvent::sm_descriptor, Events::Input::ControllerButtons::DPAD_RIGHT);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerAxisEvent(Firelight::Events::InputEvents::OnPlayerMoveUpEvent::sm_descriptor, Axis::Y, ControllerThumbsticks::LEFT, 1.0f);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerAxisEvent(Firelight::Events::InputEvents::OnPlayerMoveLeftEvent::sm_descriptor, Axis::X, ControllerThumbsticks::LEFT, -1.0f);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerAxisEvent(Firelight::Events::InputEvents::OnPlayerMoveDownEvent::sm_descriptor, Axis::Y, ControllerThumbsticks::LEFT, -1.0f);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerAxisEvent(Firelight::Events::InputEvents::OnPlayerMoveRightEvent::sm_descriptor, Axis::X, ControllerThumbsticks::LEFT, 1.0f);
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
		Engine::Instance().GetSystemManager().RegisterGameSystem<InventoryManager>();
		// Register KeyBindings
		BindDefaultKeys();

		// Camera
		CameraEntity* camera = new CameraEntity();
		Engine::Instance().SetActiveCamera(camera);

		// Player
		PlayerEntity* player = new PlayerEntity();

		// Grass
		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
		test2->GetSpriteComponent()->pixelsPerUnit = 20.0f;
		test2->GetSpriteComponent()->layer = 16;

		// UI
		UICanvas* canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0));
		PlayerHealthUI* playerHealthUI = new PlayerHealthUI(canvas);



		ParentID = canvas->GetEntityID();
		InventoryComponentGroupID group1;
		group1.Group = "PlayerInv";
		group1.isDisplayAll = true;

		InventoryWrapper* inv1 = new InventoryWrapper();
		inv1->SetGroup(group1);
		inv1->GetInvComp()->Name = "MainIven";
		inv1->GetInvComp()->offset = Maths::Vec2f(0, 0);
		inv1->GetInvComp()->Size = Maths::Vec2f(300, 720 / 2);
		inv1->GetInvComp()->RowCount = 10;
		inv1->GetInvComp()->ColoumCount = 3;
		inv1->GetInvComp()->AnchorSettings = ECS::e_AnchorSettings::TopRight;


		InventoryWrapper* inv2 = new InventoryWrapper();
		inv2->SetGroup(group1);
		inv2->GetInvComp()->Name = "equaitment";
		inv2->GetInvComp()->Size = Maths::Vec2f(300, 720 / 2);
		inv2->GetInvComp()->RowCount = 10;
		inv2->GetInvComp()->ColoumCount = 3;
		inv2->GetInvComp()->offset = Maths::Vec2f(0, (720 / 2)+100);
		inv2->GetInvComp()->AnchorSettings = ECS::e_AnchorSettings::TopRight;

		Firelight::Events::EventDispatcher::InvokeFunctions<Events::Inv::ADD_NEW_INV>();

		//open via interation
		//Firelight::Events::EventDispatcher::InvokeListeners<Events::Inv::LOAD_INVENTORY_GROUP>((void*)"PlayerInv");




		// Debug UI
		SetupDebugUI();

		// Load All Items
		ItemDatabase::Instance()->LoadItems("Assets/items.csv");

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			Maths::Vec3f desiredPosition = player->GetTransformComponent()->position;
			Maths::Vec3f smoothPosition = Maths::Vec3f::Lerp(camera->GetTransformComponent()->position, desiredPosition, 5 * Firelight::Engine::Instance().GetTime().GetDeltaTime());
			camera->GetTransformComponent()->position = smoothPosition;

			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}