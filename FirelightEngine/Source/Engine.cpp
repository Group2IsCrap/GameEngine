#include "Engine.h"

#include "Graphics/GraphicsHandler.h"
#include "Graphics/AssetManager.h"

#include "Input/ProcessInput.h"
#include "Maths/Random.h"

#include "ECS/EntityComponentSystem.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/PhysicsComponents.h"
#include "ECS/Components/RenderingComponents.h"

#include "Events/EventDispatcher.h"
#include "Graphics/GraphicsEvents.h"

#include "ECS/EntityWrappers/CameraEntity.h"

using namespace Firelight::ECS;

namespace Firelight
{
    Engine::Engine::Engine() :
        m_initialised(false)
    {
    }

    Engine::Engine::~Engine()
    {
    }

    Engine& Engine::Engine::Instance()
    {
        static Engine instance;
        return instance;
    }

    bool Engine::Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, const Maths::Vec2i& dimensions)
    {
        ASSERT_RETURN(!m_initialised, "Engine has already been initialsed", false);

        // Initialise COM library stuffs
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        COM_ERROR_FATAL_IF_FAILED(hr, "Failed to co-initialize.");

        // Initialise window container
        bool result = m_windowContainer.GetWritableWindow().Initialise(&m_windowContainer, hInstance, windowTitle, windowClass, dimensions);
        ASSERT_RETURN(result, "Window container failed to initialise", false);

        // Initialise graphics handler
        result = Graphics::GraphicsHandler::Instance().Initialize(m_windowContainer.GetWindow().GetHWND(), dimensions);
        ASSERT_RETURN(result, "GraphicsHandler failed to initialise", false);

        // Initialise asset manager
        result = Graphics::AssetManager::Instance().Initialise();
        ASSERT_RETURN(result, "AssetManager failed to initialise", false);

        // Seed random
        Maths::Random::SeedWithCurrentTime();

        m_systemManager.RegisterEngineSystems();

        m_activeCamera = new ECS::CameraEntity();

        m_initialised = true;

        return true;
    }

    void Engine::SetWindowDimensions(const Maths::Vec2i& dimensions)
    {
        // If the dimensions are already the given size or zero, don't bother updating
        if (dimensions == 0 || dimensions == GetWindowDimensions())
        {
            return;
        }

        m_windowContainer.GetWritableWindow().SetDimensions(dimensions);

        if (Graphics::GraphicsHandler::Instance().IsInitialised())
        {
            Graphics::GraphicsHandler::Instance().HandleResize(dimensions);
        }
    }

    ECS::SystemManager& Engine::GetSystemManager()
    {
        return m_systemManager;
    }
  
    bool Engine::ProcessMessages()
    {
        return m_windowContainer.GetWritableWindow().ProcessMessages();
    }

    const Maths::Vec2i& Engine::GetWindowDimensions() const
    {
        return m_windowContainer.GetWindow().GetDimensions();
    }

    const Maths::Vec2f& Engine::GetWindowDimensionsFloat() const
    {
        return m_windowContainer.GetWindow().GetDimensionsFloat();
    }

    const HWND Engine::GetWindowHandle() const
    {
        return m_windowContainer.GetWindow().GetHWND();
    }

    void Engine::UpdateActiveCamera2DRect()
    {
        if (m_activeCamera != nullptr)
        {
            const Maths::Vec3f& cameraPos = m_activeCamera->GetTransformComponent()->position;
            const Maths::Vec2f& windowDimensions = GetWindowDimensionsFloat();
            const float size = m_activeCamera->GetCamera2DComponent()->size * 0.01f;

            m_activeCameraRect = Maths::Rectf(
                cameraPos.x - windowDimensions.x * 0.5f * size,
                cameraPos.y - windowDimensions.y * 0.5f * size,
                windowDimensions.x * size, windowDimensions.y * size);
        }
    }

    const Maths::Rectf& Engine::GetActiveCamera2DRect() const
    {
        return m_activeCameraRect;
    }

    double Engine::Update()
    {
        Input::ProcessInput::Instance()->ControllerInput();

        m_frameTimer.Stop();
        double deltaTime = m_frameTimer.GetDurationSeconds();
        m_frameTimer.Start();

        m_systemManager.Update(deltaTime);

        UpdateActiveCamera2DRect();

        return deltaTime;
    }

    void Engine::RenderFrame()
    {
        Graphics::GraphicsHandler::Instance().Render();

        Events::EventDispatcher::InvokeFunctions<Events::Graphics::OnEarlyRender>();
    }
}
