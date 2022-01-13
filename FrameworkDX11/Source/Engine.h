#pragma once

#include "WindowContainer.h"

#include "Utils/Timer.h"

// Always included so that error macros work
#include "Utils/ErrorManager.h"

namespace TempEngineNamespace
{
    class TempEngineName
    {
    public:
        ~TempEngineName();

        static TempEngineName& Instance();

        bool Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int windowWidth, int windowHeight);

        bool ProcessMessages();
        void Update();
        void RenderFrame();

    private:
        TempEngineName();

    private:
        WindowContainer mWindowContainer;
        Utils::Timer    mFrameTimer;

        bool            mInitialised;
    };
}
