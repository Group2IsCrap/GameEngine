
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

	if (Engine::Instance().Initialise(hInstance, "Test Window", "windowClass", Firelight::Maths::Vec2i(1280, 720)))
	{
		// ImGui Test code
		ImGuiTestLayer* testLayer = new ImGuiTestLayer();
		ImGuiManager::Instance()->AddRenderLayer(testLayer);
		ImGuiManager::Instance()->RemoveRenderLayer(testLayer);

		Graphics::Model* model = Firelight::Graphics::AssetManager::Instance().GetModel<Firelight::Graphics::FancyLitVertex>("cube.obj");
		Graphics::Texture* textureTest = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/transparency_test.jpg");

		while (Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();

			for (int pepeIndex = 0; pepeIndex < 100; ++pepeIndex)
			{
				Maths::Vec2f pepePos = Engine::Instance().GetWindowDimensionsFloat() * 0.5f + Maths::Vec2f(2.0f, 1.0f) * Maths::Vec2f::GetRandomVector() * Engine::Instance().GetWindowDimensionsFloat().y * 0.5f;
				Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(Maths::Rectf(pepePos.x - 100.0f, pepePos.y - 100.0f, 200.0f, 200.0f), textureTest);
			}

			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}