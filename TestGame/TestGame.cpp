
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components.h"
#include "Source/ImGuiUI/ImGuiManager.h"

#include "Source/Maths/Vec3.h"
#include "Source/Maths/Vec2.h"
#include "Source/Maths/Rect.h"

#include "ImGuiTestLayer.h"

#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/Data/Model.h"
#include "Source/Graphics/Data/VertexTypes.h"

#include "Source/Graphics/SpriteBatch.h"

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
		ImGuiManager::Instance()->RemoveRenderLayer(testLayer);

		Graphics::Model* model = Graphics::AssetManager::Instance().GetModel<Graphics::FancyLitVertex>("cube.obj");
		Graphics::Texture* textureTest = Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/transparency_test.png");

		Maths::Vec2f pepePositions[100];
		Maths::Vec2f pepeVelocities[100];
		int pepeLayers[100];

		const auto& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

		for (int pepeIndex = 0; pepeIndex < 100; ++pepeIndex)
		{
			pepeVelocities[pepeIndex] = Maths::Vec2f::GetRandomVector();
			pepeLayers[pepeIndex] = (int)(Maths::Random::ZeroToOne<float>() * 64.0f);
		}

		while (Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();
			for (int pepeIndex = 0; pepeIndex < 100; ++pepeIndex)
			{
				Maths::Vec2f& pepePos = pepePositions[pepeIndex];
				pepePositions[pepeIndex] += pepeVelocities[pepeIndex];

				if (pepePos.x > windowDimensions.x + 100.0f) pepePos.x -= windowDimensions.x + 200.0f;
				if (pepePos.x < -100.0f) pepePos.x += windowDimensions.x + 200.0f;
				if (pepePos.y > windowDimensions.y + 100.0f) pepePos.y -= windowDimensions.y + 200.0f;
				if (pepePos.y < -100.0f) pepePos.y += windowDimensions.y + 200.0f;

				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(pepePositions[pepeIndex].x - 100.0f, pepePos.y - 100.0f, 200.0f, 200.0f), textureTest, pepeLayers[pepeIndex]);
			}

			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(100.0f, 100.0f, 200.0f, 200.0f), Graphics::AssetManager::Instance().GetDefaultTexture(), 0);

			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}