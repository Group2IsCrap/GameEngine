
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"

#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Entity.h"
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

		Entity* player = new Entity();
		player->AddComponent<IdentificationComponent>(new IdentificationComponent());
		player->AddComponent<TransformComponent>(new TransformComponent());
		EntityComponentSystem::Instance()->DebugEntities();

		player->AddComponent<PhysicsComponent>(new PhysicsComponent());
		EntityComponentSystem::Instance()->DebugEntities();

		player->RemoveComponent<TransformComponent>();
		EntityComponentSystem::Instance()->DebugEntities();

		IdentificationComponent* playerID = player->GetComponent<IdentificationComponent>();
		playerID->name = "Test";

		EntityComponentSystem::Instance()->DebugEntities();

		delete player;

		EntityComponentSystem::Instance()->DebugEntities();

		// ToDo: Test Remove Entity

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	return 0;
}

