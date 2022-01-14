
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	if (TempEngineNamespace::TempEngineName::Instance().Initialise(hInstance, "Test Window", "windowClass", 1280, 720))
	{
		while (TempEngineNamespace::TempEngineName::Instance().ProcessMessages())
		{
			TempEngineNamespace::TempEngineName::Instance().Update();
			TempEngineNamespace::TempEngineName::Instance().RenderFrame();
		}
	}

	return 0;
}

