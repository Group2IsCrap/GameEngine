
// Defines the entry point for the application.

#include "Source/Engine.h"

#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/AnimationComponent.h"
#include "Source/ECS/Components/PhysicsComponents.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/ECS/Systems/System.h"

#include "Source/ECS/EntityWrappers/SpriteEntity.h"
#include"Source/ECS/EntityWrappers/UIEntity.h"
#include "Source/ImGuiUI/ImGuiManager.h"

#include "ImGuiTestLayer.h"

#include "Source/Graphics/GraphicsHandler.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/SpriteBatch.h"
#include "Source/Animation/Animation.h"

#include "Source/Input/GetInput.h"

#include "Source/ECS/Systems/AnimationSystem.h"

#include "Pepe.h"


#include"Source\Events\UIEvents.h"
using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;

void ChangeThing(UIWidget* thing) {
	thing->anchorSettings = ECS::e_AnchorSettings::Top;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "Test Window", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// ImGui Test code
		ImGuiTestLayer* testLayer = new ImGuiTestLayer();
		ImGuiManager::Instance()->AddRenderLayer(testLayer);
		//ImGuiManager::Instance()->RemoveRenderLayer(testLayer);

		// Animation Test
		Firelight::Animation::Animation animation = Firelight::Animation::Animation(
			"Shrek",
			Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/shrekWalk.png"),
			35 * 6,
			43 * 6,
			6, 
			100.0f);

		SpriteEntity* animationTest = new SpriteEntity();
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/shrekWalk.png");
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 200;
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->layer = 64;
		animationTest->AddComponent<Firelight::ECS::AnimationComponent>();
		animationTest->GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(animation.m_animationName, animation));
		Firelight::ECS::AnimationSystem::Instance()->Play(animationTest, animation.m_animationName);

		animationTest->AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::CircleColliderComponent());
		animationTest->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()->radius = 1.0f;

		Graphics::Texture* glowTexture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");

		const int numPepes = 50;
		Pepe pepes[numPepes];

		const auto& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("rickastleybackground1250.jpg");
		test2->GetSpriteComponent()->pixelsPerUnit = 25.0f;
		test2->GetSpriteComponent()->layer = 16;

		UIEntity* testUICan = new UIEntity();
		testUICan->GetSpriteComponent()->texture= Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png"); 
		testUICan->GetSpriteComponent()->toDraw = false;
		testUICan->GetTransformComponent()->scale = Maths::Vec3f(1.0f, 1.0f, 0);
		testUICan->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Canvas());
		testUICan->GetComponent<Firelight::ECS::UIWidget,Firelight::ECS::UI_Canvas>()->anchorSettings= ECS::e_AnchorSettings::Center;
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->texture = testUICan->GetSpriteComponent();
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->transform = testUICan->GetTransformComponent();
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isPressable = false;
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isDrag = false;
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->isHover = false;
		testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>()->layer = 32;
		UIEntity* testUIP = new UIEntity();

		testUIP->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUIP->GetTransformComponent()->position.x = Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUIP->GetTransformComponent()->position.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		
		testUIP->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Panel());
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->anchorSettings = ECS::e_AnchorSettings::Top;
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->parent = testUICan->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Canvas>();
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->texture = testUIP->GetSpriteComponent();
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->transform = testUIP->GetTransformComponent();
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultPosition.x = Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultPosition.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isHover = false;
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isPressable = false;
		testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultScale = Maths::Vec3f(0.5f, 0.5f, 0);

		UIEntity* testUIP2 = new UIEntity();
		testUIP2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUIP2->GetTransformComponent()->position.x = Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUIP2->GetTransformComponent()->position.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUIP2->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Panel());
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->anchorSettings = ECS::e_AnchorSettings::Bottom;
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->parent = testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->texture = testUIP2->GetSpriteComponent();
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->transform = testUIP2->GetTransformComponent();
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultPosition.x = Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultPosition.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isHover = false;
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->isPressable = false;
		testUIP2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>()->defaultScale = Maths::Vec3f(0.5f, 0.5f, 0);
		UIEntity* testUI2 = new UIEntity();
		testUI2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		
		testUI2->GetTransformComponent()->position.x= Engine::Instance().GetWindowDimensionsFloat().x/2;
		testUI2->GetTransformComponent()->position.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUI2->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Button());
		testUI2->GetComponent<Firelight::ECS::UIWidget,Firelight::ECS::UI_Button>()->anchorSettings= ECS::e_AnchorSettings::Right;
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->parent = testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->texture = testUI2->GetSpriteComponent();
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->transform = testUI2->GetTransformComponent();
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultPosition.x= Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultPosition.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultScale = Maths::Vec3f(0.2f, 0.2f, 0);
		
		UIEntity* testUI3 = new UIEntity();
		testUI3->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUI3->GetTransformComponent()->position.x = Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUI3->GetTransformComponent()->position.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUI3->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Button());
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->anchorSettings = ECS::e_AnchorSettings::Center;
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->parent = testUIP->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Panel>();
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->texture = testUI3->GetSpriteComponent();
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->transform = testUI3->GetTransformComponent();
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->defaultScale = Maths::Vec3f(0.2f, 0.2f, 0);
		testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->onLeftPressFunctions.push_back(std::bind(ChangeThing, testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()));
		//testUI3->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->OffSet = (100, 100);
		Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent> ();

		SpriteEntity* collisionTest = new SpriteEntity();
		collisionTest->GetComponent<Firelight::ECS::TransformComponent>()->position.x = 10.0f;
		collisionTest->GetComponent<Firelight::ECS::SpriteComponent>()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/Circle.png");
		collisionTest->GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 50;
		collisionTest->GetComponent<Firelight::ECS::SpriteComponent>()->layer = 33;
		collisionTest->AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::CircleColliderComponent());
		collisionTest->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()->radius = 1.0f;
		collisionTest->GetComponent<Firelight::ECS::StaticComponent>()->isStatic = true;

		
		int speed = 10.0f;

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			if (Firelight::Input::InputGet.KeyIsPress('W'))
			{
				animationTest->GetTransformComponent()->position.y += Engine::Instance().GetTime().GetDeltaTime() * speed;
			}

			if (Firelight::Input::InputGet.KeyIsPress('A'))
			{
				animationTest->GetTransformComponent()->position.x -= Engine::Instance().GetTime().GetDeltaTime() * speed;
			}

			if (Firelight::Input::InputGet.KeyIsPress('S'))
			{
				animationTest->GetTransformComponent()->position.y -= Engine::Instance().GetTime().GetDeltaTime() * speed;
			}

			if (Firelight::Input::InputGet.KeyIsPress('D'))
			{
				animationTest->GetTransformComponent()->position.x += Engine::Instance().GetTime().GetDeltaTime() * speed;
			}
			
			/*for (int pepeIndex = 0; pepeIndex < numPepes; ++pepeIndex)
			{
				Pepe& pepe = pepes[pepeIndex];

				pepe.Update(deltaTime, windowDimensions);
				pepe.Draw();
			}
			

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(100.0f, 100.0f, 200.0f, 200.0f), Graphics::AssetManager::Instance().GetDefaultTexture(), 64);

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(200.0f, 200.0f, 400.0f, 400.0f), glowTexture, 48);
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(800.0f, 0.0f, 400.0f, 400.0f), glowTexture, 32);
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(400.0f, 300.0f, 400.0f, 400.0f), glowTexture, 16);*/

			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}
