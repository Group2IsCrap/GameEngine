
// Defines the entry point for the application.

#include "Source/Engine.h"
#include "Source/Utils/ErrorManager.h"
#include "Source/Maths/Vec3.inl"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Firelight::Maths::Vec3f vecTest = Firelight::Maths::Vec3f(0.0f, 1.0f, 1.0f);
	vecTest.Normalise();

	std::string vecString = std::to_string(vecTest.x) + ", " + std::to_string(vecTest.y) + ", " + std::to_string(vecTest.z);
	ERROR_STANDARD(vecString)

	if (Firelight::Engine::Instance().Initialise(hInstance, "Test Window", "windowClass", 1280, 720))
	{
		while (Firelight::Engine::Instance().ProcessMessages())
		{
			Firelight::Engine::Instance().Update();
			Firelight::Engine::Instance().RenderFrame();
		}
	}

	return 0;
}

