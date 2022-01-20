#pragma once

#include "WindowContainer.h"

#include "Utils/Timer.h"

// Always included so that error macros work
#include "Utils/ErrorManager.h"

namespace Firelight
{
    class Engine
    {
    public:
        ~Engine();

        static Engine& Instance();

        bool Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int windowWidth, int windowHeight);

        void RegisterEngineComponents();

        bool ProcessMessages();
        void Update();
        void RenderFrame();

    private:
        Engine();

    private:
        WindowContainer m_windowContainer;
        Utils::Timer    m_frameTimer;

        bool            m_initialised;
    };
}
