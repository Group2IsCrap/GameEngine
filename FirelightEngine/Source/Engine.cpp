#include "Engine.h"

#include "Graphics/GraphicsHandler.h"
#include "Input/ProcessInput.h"
#include "Maths/Random.h"

#include "ECS/EntityComponentSystem.h"
#include "ECS/Components.h"

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

    bool Engine::Initialise(HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int windowWidth, int windowHeight)
    {
        ASSERT_RETURN(!m_initialised, "Engine has already been initialsed", false);

        // Initialise COM library stuffs
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        COM_ERROR_FATAL_IF_FAILED(hr, "Failed to co-initialize.");

        // Initialise window container
        bool result = m_windowContainer.GetWindow().Initialise(&m_windowContainer, hInstance, windowTitle, windowClass, windowWidth, windowHeight);
        ASSERT_RETURN(result, "Window container failed to initialise", false);

        // Initialise graphics handler
        result = Graphics::GraphicsHandler::Instance().Initialize(m_windowContainer.GetWindow().GetHWND(), windowWidth, windowHeight);
        ASSERT_RETURN(result, "DirectXManager failed to initialise", false);

        // Seed random
        Maths::Random::SeedWithCurrentTime();

        // TODO: Initalise other systems here

        RegisterEngineComponents();

        m_initialised = true;

        return true;
    }

    void Engine::RegisterEngineComponents()
    {
        EntityComponentSystem::Instance()->RegisterComponent<IdentificationComponent>();
        EntityComponentSystem::Instance()->RegisterComponent<TransformComponent>();
        EntityComponentSystem::Instance()->RegisterComponent<PhysicsComponent>();
    }

    bool Engine::ProcessMessages()
    {
        return m_windowContainer.GetWindow().ProcessMessages();
    }

    void Engine::Update()
    {
        Input::ProcessInput::Instance()->ControllerInput();
        m_frameTimer.Stop();
        double deltaTime = m_frameTimer.GetDurationSeconds();
        m_frameTimer.Start();

        (void)deltaTime;
        // Update engine systems with deltaTime here

        Input::ProcessInput::Instance()->TestInput();
    }

    void Engine::RenderFrame()
    {
        Graphics::GraphicsHandler::Instance().Render();
    }
}
