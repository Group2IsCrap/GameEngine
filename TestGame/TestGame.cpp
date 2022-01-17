
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"
#include "Source/Maths/Vec3.inl"

#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Components.h"

#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/Data/Model.h"
#include "Source/Graphics/Data/VertexTypes.h"

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
		EntityComponentSystem::Instance()->AddComponent<IdentificationComponent>(player, new IdentificationComponent());
		EntityComponentSystem::Instance()->AddComponent<TransformComponent>(player, new TransformComponent());

		EntityComponentSystem::Instance()->DebugEntities();

		EntityComponentSystem::Instance()->AddComponent<PhysicsComponent>(player, new PhysicsComponent());
		EntityComponentSystem::Instance()->DebugEntities();

		EntityComponentSystem::Instance()->RemoveComponent<TransformComponent>(player);
		EntityComponentSystem::Instance()->DebugEntities();

		IdentificationComponent* playerID = EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(player);
		playerID->name = "Test";

		EntityComponentSystem::Instance()->DebugEntities();

		EntityComponentSystem::Instance()->RemoveEntity(player);

		EntityComponentSystem::Instance()->DebugEntities();

		Firelight::Graphics::Model* model = Firelight::Graphics::AssetManager::Instance().GetModel<Firelight::Graphics::FancyLitVertex>("cube.obj");

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	return 0;
}

