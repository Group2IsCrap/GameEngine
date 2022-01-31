#pragma once

#include <Source/ECS/Systems/System.h>

class PlayerSystem : public Firelight::ECS::System
{
public:
	PlayerSystem();
	void Update(const Firelight::Utils::Time& time) override;

private:
	void SetState(int state);

private:
	bool m_right;
	bool m_left;
	bool m_idle;
	int m_state;
};