
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

using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Firelight::Engine::Instance().Initialise(hInstance, "Test Window", "windowClass", Firelight::Maths::Vec2i(1280, 720)))
	{
		// ImGui Test code
		ImGuiTestLayer* testLayer = new ImGuiTestLayer();
		ImGuiManager::Instance()->AddRenderLayer(testLayer);
		ImGuiManager::Instance()->RemoveRenderLayer(testLayer);

		Firelight::Graphics::Model* model = Firelight::Graphics::AssetManager::Instance().GetModel<Firelight::Graphics::FancyLitVertex>("cube.obj");
		Firelight::Graphics::Texture* textureTest = Firelight::Graphics::AssetManager::Instance().GetDefaultTexture();

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();

			Firelight::Maths::Rectf dest(100.0f, 100.0f, 200.0f, 200.0f);
			Firelight::Maths::Rectf source(0.0f, 0.0f, 1024.0f, 1024.0f);

			Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(dest, source, 0, textureTest);
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	return 0;
}