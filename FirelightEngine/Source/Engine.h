#pragma once

#include "WindowContainer.h"

#include "Maths/Rect.h"

#include "Utils/Time.h"

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

        const Utils::Time&  GetTime() const;
        Utils::Time&        GetWritableTime();

        void                UpdateActiveCamera2DRect();
        const Maths::Rectf& GetActiveCamera2DRect() const;
        ECS::CameraEntity*  GetActiveCamera() const;
        void                SetActiveCamera(ECS::CameraEntity* camera);

        bool   ProcessMessages();
        void   Update();

        void   RenderFrame();

    private:
        Engine();

    private:
        Utils::Time        m_time;

        ECS::SystemManager m_systemManager;

        WindowContainer    m_windowContainer;

        bool               m_initialised;

        ECS::CameraEntity* m_activeCamera;
        Maths::Rectf       m_activeCameraRect;

        double             m_runTime;

        double             m_physicsTimeStep;
        double             m_physicsTime;
    };
}
