#include "Source/Engine.h"
#include "Source/ImGuiUI/ImGuiManager.h"
#include <Source/ECS/Components.h>
#include <Source/ECS/EntityComponentSystem.h>

#include "ImGuiEditorLayer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Firelight::Engine::Instance().Initialise(hInstance, "Firelight Editor", "windowClass", 1920-100, 1080-100))
	{
		// ImGui Test code
		ImGuiEditorLayer* testLayer = new ImGuiEditorLayer();
		Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(testLayer);

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}

		delete testLayer;
	}

	

	return 0;
}