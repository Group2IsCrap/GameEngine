#pragma once

#include "WindowContainer.h"

#include "Maths/Vec2.h"

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

        bool Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, const Maths::Vec2i& dimensions);

        void                SetWindowDimensions(const Maths::Vec2i& dimensions);

        const Maths::Vec2i& GetWindowDimensions() const;
        const Maths::Vec2f& GetWindowDimensionsFloat() const;

        const HWND          GetWindowHandle() const;

        bool   ProcessMessages();
        double Update();
        void   RenderFrame();

    private:
        Engine();

    private:
        WindowContainer m_windowContainer;
        Utils::Timer    m_frameTimer;

        bool            m_initialised;
    };
}
