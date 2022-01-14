#include "Engine.h"

namespace TempEngineNamespace
{
    TempEngineName::TempEngineName::TempEngineName() :
        mInitialised(false)
    {
    }

    TempEngineName::TempEngineName::~TempEngineName()
    {
    }

    TempEngineName& TempEngineName::TempEngineName::Instance()
    {
        static TempEngineName instance;
        return instance;
    }

    bool TempEngineName::Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int windowWidth, int windowHeight)
    {
        ASSERT_RETURN(!mInitialised, "Engine has already been initialsed", false);

        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        COM_ERROR_FATAL_IF_FAILED(hr, "Failed to co-initialize.");

        // Initialise window container
        bool result = mWindowContainer.GetWindow().Initialise(&mWindowContainer, hInstance, windowTitle, windowClass, windowWidth, windowHeight);
        ASSERT_RETURN(result, "Window container failed to initialise", false);

        // TODO: Initalise other systems here

        mInitialised = true;

        return true;
    }

    bool TempEngineName::ProcessMessages()
    {
        return mWindowContainer.GetWindow().ProcessMessages();
    }

    void TempEngineName::Update()
    {
        mFrameTimer.Stop();
        float deltaTime = mFrameTimer.GetDurationSeconds();
        mFrameTimer.Start();

        (void)deltaTime;
        // Update engine systems with deltaTime here
    }

    void TempEngineName::RenderFrame()
    {
    }
}
