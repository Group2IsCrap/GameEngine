#include "Source/Engine.h"
#include "Source/ImGuiUI/ImGuiManager.h"
#include <Source/ECS/Components.h>
#include <Source/ECS/EntityComponentSystem.h>

#include "ImGuiEditorLayer.h"

using namespace Firelight::ECS;
using namespace Firelight::ImGuiUI;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Register components required
	EntityComponentSystem::Instance()->RegisterComponent<IdentificationComponent>();
	EntityComponentSystem::Instance()->RegisterComponent<TransformComponent>();
	EntityComponentSystem::Instance()->RegisterComponent<PhysicsComponent>();

	// ImGui Test code
	ImGuiEditorLayer* testLayer = new ImGuiEditorLayer();
	ImGuiManager::Instance()->AddRenderLayer(testLayer);

	if (Firelight::Engine::Instance().Initialise(hInstance, "Firelight Editor", "windowClass", 1280, 720))
	{
		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	delete testLayer;

	return 0;
}