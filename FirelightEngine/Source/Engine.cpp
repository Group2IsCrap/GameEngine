#include "Engine.h"

#include "Graphics/GraphicsHandler.h"
#include "Graphics/AssetManager.h"

#include "Input/ProcessInput.h"
#include "Maths/Random.h"

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

        // TODO: Initalise other systems here

        m_initialised = true;

        return true;
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

    double Engine::Update()
    {
        Input::ProcessInput::Instance()->ControllerInput();
        m_frameTimer.Stop();
        double deltaTime = m_frameTimer.GetDurationSeconds();
        m_frameTimer.Start();

        (void)deltaTime;
        // Update engine systems with deltaTime here

        Input::ProcessInput::Instance()->TestInput();

        return deltaTime;
    }

    void Engine::RenderFrame()
    {
        Graphics::GraphicsHandler::Instance().Render();
    }
}
