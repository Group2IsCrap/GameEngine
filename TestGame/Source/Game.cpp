
// Defines the entry point for the application.

#include "Source/Engine.h"

#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/AnimationComponent.h"
#include "Source/ECS/Components/PhysicsComponents.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/ECS/Systems/System.h"
#include "Source/Maths/Rect.h"

#include "Source/ECS/EntityWrappers/SpriteEntity.h"
#include"Source/ECS/EntityWrappers/UIEntity.h"
#include "Source/ImGuiUI/ImGuiManager.h"

#include "Source/Graphics/GraphicsHandler.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/SpriteBatch.h"
#include "Source/Animation/Animation.h"

#include "Source/Input/GetInput.h"

#include "Source/ECS/Systems/AnimationSystem.h"

#include "FModAudio.h"

#include "UI/HealthUI.h"


#include"Source\Events\UIEvents.h"
#include "Player/PlayerSystem.h"
#include "Player/PlayerEntity.h"
#include "Components/PlayerComponent.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;
using namespace snowFallAudio::FModAudio;

static UIEntity* s_uiCanvas;

void PlaySound_Internal(const std::string& soundName, const Vector3D& soundPos, float volumedB)
{
	snowFallAudio::FModAudio::AudioEngine::engine->PlayfModSound(soundName, soundPos, volumedB);
}

void PlayBeuu()
{
	PlaySound_Internal("beeuuuuu.mp3", Vector3D(0.0f, 0.0f, 0.0f), 0.5f);
}

void PlayMusic()
{
	PlaySound_Internal("sound.mp3", Vector3D(0.0f, 0.0f, 0.0f), 0.1f);

}

void CreatUITest() {
	//needs new immages
	s_uiCanvas = new UIEntity();
	s_uiCanvas->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/CanvasTest.png");
	s_uiCanvas->GetSpriteComponent()->toDraw = false;
	s_uiCanvas->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Canvas());
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->anchorSettings = ECS::e_AnchorSettings::Center;
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->texture = s_uiCanvas->GetSpriteComponent();
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->transform = s_uiCanvas->GetTransformComponent();
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->layer = 100;
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isDraggable = false;
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isPressable = false;
	s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isHoverable = false;

	// Health UI
	HealthUI* healthUI = new HealthUI(s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>());

	UIEntity* panelSound = new UIEntity();
	panelSound->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	panelSound->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Panel());
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->anchorSettings = ECS::e_AnchorSettings::Right;
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->parent = s_uiCanvas->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>();
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->texture = panelSound->GetSpriteComponent();
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->transform = panelSound->GetTransformComponent();
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isHoverable = true;
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isPressable = true;
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isDraggable = true;
	panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultScale = Maths::Vec3f(0.1f, 0.5f, 0);

	UIEntity* buttonSound = new UIEntity();
	buttonSound->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/ButtionTest.png");
	buttonSound->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Button());
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->anchorSettings = ECS::e_AnchorSettings::Center;
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->parent = panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->texture = buttonSound->GetSpriteComponent();
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->transform = buttonSound->GetTransformComponent();
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultScale = Maths::Vec3f(0.5f, 0.1f, 0);
	buttonSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->onLeftPressFunctions.push_back(std::bind(PlayBeuu));

	UIEntity* buttonMusic = new UIEntity();
	buttonMusic->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/ButtionTest.png");
	buttonMusic->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Button());
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->anchorSettings = ECS::e_AnchorSettings::Center;
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->parent = panelSound->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->texture = buttonMusic->GetSpriteComponent();
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->transform = buttonMusic->GetTransformComponent();
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultScale = Maths::Vec3f(0.5f, 0.1f, 0);
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->onLeftPressFunctions.push_back(std::bind(PlayMusic));
	buttonMusic->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->offSet = Firelight::Maths::Vec2f(0.0f, 50.0f);

	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "GameSmiths - Vertical Slice", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// Register Systems
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<PlayerSystem>();

		// Player Character
		float playerSpeed = 10.0f;
		PlayerEntity* player = new PlayerEntity(playerSpeed);

		Graphics::Texture* glowTexture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");

		const auto& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
		test2->GetSpriteComponent()->pixelsPerUnit = 20.0f;
		test2->GetSpriteComponent()->layer = 16;

		CreatUITest();

		SpriteEntity* barn = new SpriteEntity();
		barn->GetComponent<Firelight::ECS::TransformComponent>()->position.x = 7.0f;
		barn->GetComponent<Firelight::ECS::TransformComponent>()->position.y = 5.0f;
		barn->GetComponent<Firelight::ECS::SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/barn.png");
		barn->GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 50;
		barn->GetComponent<Firelight::ECS::SpriteComponent>()->layer = 33;
		barn->AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent());
		barn->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 8.0f, 7.0f);
		barn->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->drawCollider = true;
		barn->GetComponent<Firelight::ECS::StaticComponent>()->isStatic = true;

		SpriteEntity* circle = new SpriteEntity();
		circle->GetComponent<Firelight::ECS::TransformComponent>()->position.x = -7.0f;
		circle->GetComponent<Firelight::ECS::TransformComponent>()->position.y = -5.0f;
		circle->GetComponent<Firelight::ECS::SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/ball.png");
		circle->GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 50;
		circle->GetComponent<Firelight::ECS::SpriteComponent>()->layer = 32;
		circle->AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent());
		circle->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 2.0f, 2.0f);
		circle->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->drawCollider = true;
		circle->GetComponent<Firelight::ECS::StaticComponent>()->isStatic = false;

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();
			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}
