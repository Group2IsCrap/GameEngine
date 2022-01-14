#include "Engine.h"

#include "Graphics/GraphicsHandler.h"
#include"Input/ProcessInput.h"
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

        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        COM_ERROR_FATAL_IF_FAILED(hr, "Failed to co-initialize.");

        // Initialise window container
        bool result = m_windowContainer.GetWindow().Initialise(&m_windowContainer, hInstance, windowTitle, windowClass, windowWidth, windowHeight);
        ASSERT_RETURN(result, "Window container failed to initialise", false);

        result = Graphics::GraphicsHandler::Instance().Initialize(m_windowContainer.GetWindow().GetHWND(), windowWidth, windowHeight);
        ASSERT_RETURN(result, "DirectXManager failed to initialise", false);

        // TODO: Initalise other systems here

        m_initialised = true;

        return true;
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
