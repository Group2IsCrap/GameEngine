
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"

#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components.h"

using namespace Firelight::ECS;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Firelight::Engine::Instance().Initialise(hInstance, "Test Window", "windowClass", 1280, 720))
	{
		EntityComponentSystem::Instance()->RegisterComponent<IdentificationComponent>();
		EntityComponentSystem::Instance()->RegisterComponent<TransformComponent>();
		EntityComponentSystem::Instance()->RegisterComponent<PhysicsComponent>();

		Entity player = EntityComponentSystem::Instance()->CreateEntity();
		EntityComponentSystem::Instance()->AddComponent<IdentificationComponent>(player, IdentificationComponent());
		EntityComponentSystem::Instance()->AddComponent<TransformComponent>(player, TransformComponent());
		EntityComponentSystem::Instance()->AddComponent<PhysicsComponent>(player, PhysicsComponent());

		Entity log = EntityComponentSystem::Instance()->CreateEntity();
		EntityComponentSystem::Instance()->AddComponent<IdentificationComponent>(log, IdentificationComponent());
		EntityComponentSystem::Instance()->AddComponent<PhysicsComponent>(log, PhysicsComponent());

		Entity renderable = EntityComponentSystem::Instance()->CreateEntity();
		EntityComponentSystem::Instance()->AddComponent<TransformComponent>(renderable, TransformComponent());
		EntityComponentSystem::Instance()->DebugEntities();

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	return 0;
}

