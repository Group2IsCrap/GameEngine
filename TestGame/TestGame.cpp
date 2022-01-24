
// Defines the entry point for the application.

#include "Source/Engine.h"

#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/AnimationComponent.h"
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

#include "Source/ECS/Systems/AnimationSystem.h"

#include "Pepe.h"

#include"TestInputGame.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;

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
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->texture = animation.m_texture;
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 35 * 6;
		animationTest->GetComponent<Firelight::ECS::SpriteComponent>()->layer = 64;
		animationTest->AddComponent<Firelight::ECS::AnimationComponent>();
		animationTest->GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(animation.m_animationName, animation));
		Firelight::ECS::AnimationSystem::Instance()->Play(animationTest, animation.m_animationName);

		Graphics::Texture* glowTexture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");

		const int numPepes = 50;
		Pepe pepes[numPepes];

		const auto& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

		SpriteEntity* test2 = new SpriteEntity();
		test2->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("rickastleybackground1250.jpg");
		test2->GetSpriteComponent()->pixelsPerUnit = 25.0f;
		test2->GetSpriteComponent()->layer = 16;

		UIEntity* testUI = new UIEntity();
		testUI->GetSpriteComponent()->texture= Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUI->GetTransformComponent()->scale = Maths::Vec3f(0.5, 0.5, 0);
		testUI->GetSpriteComponent()->layer = 0;
		//testUI->GetSpriteComponent()->sourceRect = Maths::Rectf(100, 100, 200, 200);



		testUI = new UIEntity();
		testUI->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");
		testUI->GetTransformComponent()->scale = Maths::Vec3f(0.5, 0.5, 0);
		testUI->GetTransformComponent()->position = Maths::Vec3f(1, 0, 0);
		testUI->GetSpriteComponent()->layer = 0;

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			double deltaTime = Engine::Instance().Update();
			
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
