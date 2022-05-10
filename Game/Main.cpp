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

#include "Source/PCG/BiomeGeneration.h"

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

void GenerateBiomeUI()
{
	BiomeGeneration::Instance()->Render();
}


void SetupDebugUI()
{
	// ImGui Test code
	ImGuiDebugLayer* itemTestLayer = new ImGuiDebugLayer();
	itemTestLayer->spawnItemCommand[0] = std::bind(SpawnItem0);
	itemTestLayer->spawnItemCommand[1] = std::bind(SpawnItem1);
	itemTestLayer->spawnItemCommand[2] = std::bind(GenerateBiomeUI);
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(itemTestLayer);
}

void BindDefaultKeys()
{
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveUpEvent::sm_descriptor, Keys::KEY_W);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveLeftEvent::sm_descriptor, Keys::KEY_A);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveDownEvent::sm_descriptor, Keys::KEY_S);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnPlayerMoveRightEvent::sm_descriptor, Keys::KEY_D);
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::SpawnItemEvent::sm_descriptor, Keys::KEY_E, KeyEventType::KeyPressSingle);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerActionEvent(Firelight::Events::InputEvents::SpawnItemEvent::sm_descriptor, ControllerButtons::A, ControllerEventType::ButtonPressSingle);
	Firelight::Engine::Instance().GetKeyBinder().BindControllerAxisEvent(Firelight::Events::InputEvents::OnPlayerMoveEvent::sm_descriptor, ControllerThumbsticks::LEFT);
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

		// Register KeyBindings
		BindDefaultKeys();

		// Camera
		CameraEntity* camera = new CameraEntity();
		Engine::Instance().SetActiveCamera(camera);

		// Player
		PlayerEntity* player = new PlayerEntity();

		//Biome Generation Testing LOL
		BiomeGeneration::Instance()->Initialise();

		// Grass
		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
		test2->GetSpriteComponent()->pixelsPerUnit = 20.0f;
		test2->GetSpriteComponent()->layer = 16;

		// UI
		UICanvas* canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0));
		PlayerHealthUI* playerHealthUI = new PlayerHealthUI(canvas);

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