
// Defines the entry point for the application.

#include "Source/Engine.h"

#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Systems/System.h"

#include "Source/ImGuiUI/ImGuiManager.h"

#include "ImGuiTestLayer.h"

#include "Source/Graphics/GraphicsHandler.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/SpriteBatch.h"

#include "Pepe.h"

#include"Source/ECS/SystemManager.h"
#include"Source/UI/UISystem.h"
#include"Source\ECS\Components\UIComponents.h"
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

		Graphics::Texture* glowTexture = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/non_binary_transparency.png");

		const int numPepes = 50;
		Pepe pepes[numPepes];

		const auto& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

		
		
		Entity* test = new Entity();
		test->AddComponent<ECS::UIWidget>();
		test->AddComponent<ECS::SpriteComponent>();
		test->AddComponent<ECS::TransformComponent>();

		test->GetComponent<ECS::UIWidget>()->Textuer = test->GetComponent<ECS::SpriteComponent>();
		test->GetComponent<ECS::UIWidget>()->Transform = test->GetComponent<ECS::TransformComponent>();

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			double deltaTime = Engine::Instance().Update();
			
			for (int pepeIndex = 0; pepeIndex < numPepes; ++pepeIndex)
			{
				Pepe& pepe = pepes[pepeIndex];

				pepe.Update(deltaTime, windowDimensions);
				pepe.Draw();
			}

			inputTest->Draw();
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(100.0f, 100.0f, 200.0f, 200.0f), Graphics::AssetManager::Instance().GetDefaultTexture(), 64);

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(200.0f, 200.0f, 400.0f, 400.0f), glowTexture, 48);
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(800.0f, 0.0f, 400.0f, 400.0f), glowTexture, 32);
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(400.0f, 300.0f, 400.0f, 400.0f), glowTexture, 16);

			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}
