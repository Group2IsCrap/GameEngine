
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

#include"TestInputGame.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;

void ChangeThing(UIWidget* thing) {
	//thing->DockSettings = ECS::e_DockSettings::DockCenter;
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
		
		TestInputGame* inputTest = new TestInputGame();

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

		UIEntity* testUI2 = new UIEntity();
		testUI2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUI2->GetTransformComponent()->scale = Maths::Vec3f(0.5f, 0.5f, 0);
		testUI2->GetTransformComponent()->position.x= Engine::Instance().GetWindowDimensionsFloat().x/2;
		testUI2->GetTransformComponent()->position.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;
		testUI2->GetSpriteComponent()->layer = 20;
		testUI2->AddComponent<Firelight::ECS::UIWidget>(new Firelight::ECS::UI_Button());
		//testUI2->GetComponent<Firelight::ECS::UIWidget,Firelight::ECS::UI_Immage>()->DockSettings= ECS::e_AnchorSettings::Top;
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->Textuer = testUI2->GetSpriteComponent();
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->Transform = testUI2->GetTransformComponent();
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->DefaultPosition.x= Engine::Instance().GetWindowDimensionsFloat().x / 2;
		testUI2->GetComponent<Firelight::ECS::UIWidget, Firelight::ECS::UI_Button>()->DefaultPosition.y = Engine::Instance().GetWindowDimensionsFloat().y / 2;

		//UIEntity* testUI = new UIEntity();
		//testUI->GetSpriteComponent()->texture= Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		//testUI->GetTransformComponent()->scale = Maths::Vec3f(1.0, 1.0, 0);
		//testUI->GetSpriteComponent()->layer = 20;
		//testUI->GetUIWidget()->OnLeftPressFunctions.push_back(std::bind(ChangeThing, testUI2->GetUIWidget()));
		////testUI->GetSpriteComponent()->sourceRect = Maths::Rectf(100, 100, 200, 200);



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
