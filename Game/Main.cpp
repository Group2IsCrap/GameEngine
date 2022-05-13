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

	keyBinder->BindKeyboardActionEvent(OnInteractEvent::sm_descriptor, Keys::KEY_F);
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

		// World
		WorldEntity* world = new WorldEntity();

		// UI
		UICanvas* canvas = new UICanvas(Firelight::Maths::Vec3f(1920, 1080, 0), static_cast<int>(RenderLayer::UI));
		PlayerHealthUI* playerHealthUI = new PlayerHealthUI(canvas, player->GetHealthComponent()->maxHealth);
		DeathMenu* deathMenu = new DeathMenu(canvas);

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

		Serialiser::SaveSceneJSON();
	}

	return 0;
}