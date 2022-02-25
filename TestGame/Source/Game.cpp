
// Defines the entry point for the application.

#include "Source/Engine.h"

#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/AnimationComponent.h"
#include "Source/ECS/Components/RenderingComponents.h"
#include "Source/ECS/Components/PhysicsComponents.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/ECS/Systems/System.h"
#include "Source/Maths/Rect.h"

#include "Source/ECS/EntityWrappers/SpriteEntity.h"
#include"Source/ECS/EntityWrappers/UIEntity.h"
#include"Source/ECS/EntityWrappers/UIButton.h"
#include"Source/ECS/EntityWrappers/UICanvas.h"
#include"Source/ECS/EntityWrappers/UIPanel.h"
#include "Source/ImGuiUI/ImGuiManager.h"
#include "ImGuiItemLayer.h"

#include "Source/Graphics/GraphicsHandler.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/SpriteBatch.h"
#include "Source/Animation/Animation.h"

#include "Source/Input/GetInput.h"

#include "Source/ECS/Systems/AnimationSystem.h"

#include "FModAudio.h"

#include "UI/HealthUI.h"

#include "Source/Serialisation/Serialiser.h"

#include"Source\Events\UIEvents.h"
#include "Player/PlayerSystem.h"
#include "Player/PlayerEntity.h"
#include "Components/PlayerComponent.h"
#include "Items/ItemDatabase.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::Serialisation;
using namespace Firelight::ImGuiUI;
using namespace snowFallAudio::FModAudio;

static UICanvas* s_uiCanvas;

void PlaySound_Internal(const std::string& soundName, const Vector3D& soundPos, float volumedB)
{
	snowFallAudio::FModAudio::AudioEngine::engine->PlayfModSound(soundName, soundPos, volumedB);
}

float vol = 1.0f;
float newVol = 100.f;

void PlayBeuu()
{
	vol = 80.0f;
	PlaySound_Internal("beeuuuuu.mp3", Vector3D(0.0f, 0.0f, 0.0f), vol);
}

void PlayMusic()
{
	vol = 50.0f;
	PlaySound_Internal("sound.mp3", Vector3D(0.0f, 0.0f, 0.0f),vol);
}

void VolUp()
{
	newVol += 50.0f;
	snowFallAudio::FModAudio::AudioEngine::engine->VolumeChange(newVol);
}

void VolDown()
{
	if (newVol > 0.0f)
	{
		newVol -= 50.f;
		snowFallAudio::FModAudio::AudioEngine::engine->VolumeChange(newVol);
	}
}

void StopSounds()
{
	snowFallAudio::FModAudio::AudioEngine::engine->StopAllChannels();
}

void CreatUITest() {
	//needs new images
	s_uiCanvas = new UICanvas();
	s_uiCanvas->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/CanvasTest.png");
	s_uiCanvas->GetSpriteComponent()->toDraw = false;
	s_uiCanvas->SetAnchorSettings(ECS::e_AnchorSettings::Center);
	s_uiCanvas->GetCanvasComponent()->XScreenSize = Engine::Instance().GetWindowDimensionsFloat().x;
	s_uiCanvas->GetCanvasComponent()->YScreenSize = Engine::Instance().GetWindowDimensionsFloat().y;
	s_uiCanvas->GetCanvasComponent()->layer = 100;

	// Health UI
	HealthUI* healthUI = new HealthUI(s_uiCanvas);

	Maths::Vec2f ScreenSize = Engine::Instance().GetWindowDimensionsFloat();

	UIPanel* panelSound = new UIPanel();
	panelSound->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PanelTest.png");
	panelSound->SetAnchorSettings(ECS::e_AnchorSettings::Right);
	panelSound->SetParent(s_uiCanvas->GetEntityID());
	panelSound->SetDefaultDimensions(Maths::Vec3f(200, 400, 0));

	UIButton* buttonSound = new UIDraggableButton();
	buttonSound->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PlayButton.png");
	buttonSound->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	buttonSound->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	buttonSound->SetOffset(Maths::Vec2f(0.0f, 10.0f));
	buttonSound->BindOnLeftPressed(std::bind(PlayBeuu));
	buttonSound->SetParent(panelSound->GetEntityID());

	UIButton* buttonMusic = new UIDraggableButton();
	buttonMusic->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/MusicButton.png");
	buttonMusic->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	buttonMusic->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	buttonMusic->SetOffset(Maths::Vec2f(0.0f, 90.0f));
	buttonMusic->BindOnLeftPressed(std::bind(PlayMusic));
	buttonMusic->SetParent(panelSound->GetEntityID());

	UIButton* buttonVolUp = new UIDraggableButton();
	buttonVolUp->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/VolUpButton.png");
	buttonVolUp->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	buttonVolUp->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	buttonVolUp->SetOffset(Maths::Vec2f(0.0f, 170.0f));
	buttonVolUp->BindOnLeftPressed(std::bind(VolUp));
	buttonVolUp->SetParent(panelSound->GetEntityID());

	UIButton* buttonVolDown = new UIDraggableButton();
	buttonVolDown->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/VolDownButton.png");
	buttonVolDown->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	buttonVolDown->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	buttonVolDown->SetOffset(Maths::Vec2f(0.0f, 250.0f));
	buttonVolDown->BindOnLeftPressed(std::bind(VolDown));
	buttonVolDown->SetParent(panelSound->GetEntityID());

	UIButton* buttonStopPlay = new UIDraggableButton();
	buttonStopPlay->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/StopButton.png");
	buttonStopPlay->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	buttonStopPlay->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	buttonStopPlay->SetOffset(Maths::Vec2f(0.0f, 330.0f));
	buttonStopPlay->BindOnLeftPressed(std::bind(StopSounds));
	buttonStopPlay->SetParent(panelSound->GetEntityID());

	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

}

void SpawnItem0()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(0);
}
void SpawnItem1()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(1);
}
void SpawnItem2()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(2);
}
void SpawnItem3()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(3);
}
void SpawnItem4()
{
	ItemDatabase::Instance()->CreateInstanceOfItem(4);
}

void SetupDebugUI()
{
	// ImGui Test code
	ImGuiDebugLayer* itemTestLayer = new ImGuiDebugLayer();
	itemTestLayer->spawnItemCommand[0] = std::bind(SpawnItem0);
	itemTestLayer->spawnItemCommand[1] = std::bind(SpawnItem1);
	itemTestLayer->spawnItemCommand[2] = std::bind(SpawnItem2);
	itemTestLayer->spawnItemCommand[3] = std::bind(SpawnItem3);
	itemTestLayer->spawnItemCommand[4] = std::bind(SpawnItem4);
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(itemTestLayer);
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

		//SpriteEntity* test2 = new SpriteEntity();
		//test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
		//test2->GetSpriteComponent()->pixelsPerUnit = 20.0f;
		//test2->GetSpriteComponent()->layer = 16;

		CreatUITest();

		SetupDebugUI();

		GameEntity* text = new GameEntity();
		text->AddComponent<TextComponent>();

		text->GetComponent<TextComponent>()->text.SetString("Epic String");
		text->GetComponent<TextComponent>()->text.SetTextHeight(50.0f);

		text->GetComponent<TextComponent>()->text.SetTextAnchor(Graphics::TextAnchor::e_MidMid);

		text->GetComponent<TransformComponent>()->position = Maths::Vec3f(640.0f, 300.0f, 0.0f);

		//SpriteEntity* barn = new SpriteEntity();
		//barn->GetComponent<TransformComponent>()->position.x = 7.0f;
		//barn->GetComponent<TransformComponent>()->position.y = 5.0f;
		//barn->GetComponent<SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/barn.png");
		//barn->GetComponent<SpriteComponent>()->pixelsPerUnit = 50;
		//barn->GetComponent<SpriteComponent>()->layer = 33;
		//barn->AddComponent<ColliderComponent>(new BoxColliderComponent());
		//barn->GetComponent<ColliderComponent, BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 8.0f, 7.0f);
		//barn->GetComponent<ColliderComponent, BoxColliderComponent>()->drawCollider = true;
		//barn->GetComponent<StaticComponent>()->isStatic = true;

		//SpriteEntity* circle = new SpriteEntity();
		//circle->GetComponent<TransformComponent>()->position.x = -7.0f;
		//circle->GetComponent<TransformComponent>()->position.y = -5.0f;
		//circle->GetComponent<SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/ball.png");
		//circle->GetComponent<SpriteComponent>()->pixelsPerUnit = 50;
		//circle->GetComponent<SpriteComponent>()->layer = 32;
		//circle->AddComponent<ColliderComponent>(new BoxColliderComponent());
		//circle->GetComponent<ColliderComponent, BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 2.0f, 2.0f);
		//circle->GetComponent<ColliderComponent, BoxColliderComponent>()->drawCollider = true;
		//circle->GetComponent<StaticComponent>()->isStatic = false;

		ItemDatabase::Instance()->LoadItems("Assets/items.csv");

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();
			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}

		Serialiser::SaveSceneJSON();
	}

	return 0;
}
