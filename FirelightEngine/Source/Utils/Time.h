#pragma once

#include "Timer.h"

namespace Firelight::Utils
{
    class Time
    {
    public:
        Time();
        ~Time();

        void   Update();

        void   ResetGameTime();

        void   SetMaxDeltaTime(double maxDeltaTime);

        double GetRunTime() const;
        double GetGameTime() const;
        double GetPhysicsTime() const;

        double GetDeltaTime() const;
        double GetGameDeltaTime() const;

        void   SetPhysicsTimeStep(double physicsTimeStep);
        double GetPhysicsTimeStep() const;

        int    GetNumPhysicsUpdatesThisFrame() const;
        
        void   SetGamePaused(bool paused);
        bool   IsGamePaused() const;

        void   SetGameSpeed(double gameSpeed);
        double GetGameSpeed() const;
        

    private:
        Utils::Timer m_timer;

        double m_maxDeltaTime;

        double m_runTime;
        double m_gameTime;
        double m_physicsTime;

        double m_deltaTime;
        double m_gameDeltaTime;
        double m_physicsTimeStep;

        int    m_numPhysicsUpdatesThisFrame;

        double m_gameSpeed;
        bool   m_gamePaused;
    };
}
