#pragma once

#include "WindowContainer.h"

#include "Maths/Rect.h"

#include "Utils/Timer.h"

// Always included so that error macros work
#include "Utils/ErrorManager.h"

#include "ECS/SystemManager.h"

namespace Firelight::ECS
{
    class CameraEntity;
}

namespace Firelight
{
    class Engine
    {
    public:
        ~Engine();

        static Engine& Instance();

        bool                Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, const Maths::Vec2i& dimensions);

        void                SetWindowDimensions(const Maths::Vec2i& dimensions);

        ECS::SystemManager& GetSystemManager();

        const Maths::Vec2i& GetWindowDimensions() const;
        const Maths::Vec2f& GetWindowDimensionsFloat() const;

        const HWND          GetWindowHandle() const;

        void                UpdateActiveCamera2DRect();
        const Maths::Rectf& GetActiveCamera2DRect() const;

        bool   ProcessMessages();
        double Update();

        void   RenderFrame();

    private:
        Engine();

    private:
        ECS::SystemManager m_systemManager;

        WindowContainer    m_windowContainer;
        Utils::Timer       m_frameTimer;

        bool               m_initialised;

        ECS::CameraEntity* m_activeCamera;
        Maths::Rectf       m_activeCameraRect;
    };
}
