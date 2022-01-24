#include "Time.h"

namespace Firelight::Utils
{
    Time::Time() :
        m_maxDeltaTime(0.25),

        m_runTime(0.0),
        m_gameTime(0.0),
        m_physicsTime(0.0),

        m_deltaTime(0.0),
        m_gameDeltaTime(0.0),
        m_physicsTimeStep(0.05),
        m_numPhysicsUpdatesThisFrame(0),
        
        m_gameSpeed(1.0),
        m_gamePaused(false)
    {
    }

    Time::~Time()
    {
    }

    void Time::Update()
    {
        // Get deltaTime for this frame
        m_timer.Stop();
        m_deltaTime = m_timer.GetDurationSeconds();
        m_timer.Start();

        // Cap deltaTime
        if (m_deltaTime > m_maxDeltaTime)
        {
            m_deltaTime = m_maxDeltaTime;
        }

        // Update the time the app has been running
        m_runTime += m_deltaTime;

        // Zero game time step related vars
        m_gameDeltaTime = 0.0;
        m_numPhysicsUpdatesThisFrame = 0;
        
        // Update game related time vars if not paused
        if (!m_gamePaused)
        {
            m_gameDeltaTime = m_deltaTime * m_gameSpeed;
            m_gameTime += m_gameDeltaTime;

            while (m_physicsTime < m_gameTime - m_physicsTimeStep)
            {
                ++m_numPhysicsUpdatesThisFrame;

                m_physicsTime += m_physicsTimeStep;
            }
        }
    }

    void Time::ResetGameTime()
    {
        m_gameTime = 0.0;
        m_physicsTime = 0.0;
    }

    void Time::SetMaxDeltaTime(double maxDeltaTime)
    {
        m_maxDeltaTime = maxDeltaTime;
    }

    double Time::GetRunTime() const
    {
        return m_runTime;
    }

    double Time::GetGameTime() const
    {
        return m_gameTime;
    }

    double Time::GetPhysicsTime() const
    {
        return m_physicsTime;
    }

    double Time::GetDeltaTime() const
    {
        return m_deltaTime;
    }

    double Time::GetGameDeltaTime() const
    {
        return m_gameDeltaTime;
    }

    void Time::SetPhysicsTimeStep(double physicsTimeStep)
    {
        m_physicsTimeStep = physicsTimeStep;
    }

    double Time::GetPhysicsTimeStep() const
    {
        return m_physicsTimeStep;
    }

    int Time::GetNumPhysicsUpdatesThisFrame() const
    {
        return m_numPhysicsUpdatesThisFrame;
    }

    void Time::SetGamePaused(bool paused)
    {
        m_gamePaused = paused;
    }

    bool Time::IsGamePaused() const
    {
        return m_gamePaused;
    }

    void Time::SetGameSpeed(double gameSpeed)
    {
        m_gameSpeed = gameSpeed;
    }

    double Time::GetGameSpeed() const
    {
        return m_gameSpeed;
    }
}
