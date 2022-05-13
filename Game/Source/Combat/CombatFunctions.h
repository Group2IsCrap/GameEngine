#pragma once
class CombatFunctions : public Firelight::ECS::System, Firelight::Events::Listener
{
private:
	void DetectHit();
};

