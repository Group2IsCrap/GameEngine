#include "Source/Engine.h"

#include "FModAudio.h"

#include "Source/Systems/PlayerSystem.h"
#include "Source/Player/PlayerEntity.h"

using namespace Firelight;
using namespace Firelight::ECS;
using namespace snowFallAudio::FModAudio;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (Engine::Instance().Initialise(hInstance, "Game Name", "windowClass", Maths::Vec2i(1280, 720)))
	{
		// Register Systems
		Firelight::Engine::Instance().GetSystemManager().RegisterGameSystem<PlayerSystem>();

		// Player
		PlayerEntity* player = new PlayerEntity();

		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Engine::Instance().Update();
			snowFallAudio::FModAudio::AudioEngine::engine->Update();
			Engine::Instance().RenderFrame();
		}
	}

	return 0;
}