#pragma once

#include <Source/Utils/Time.h>

/// <summary>
/// Base class for AI state behaviour
/// </summary>
class AIStateBehaviour
{
public:
	AIStateBehaviour();

	virtual void HandleState(class AIEntity* entity, const Firelight::Utils::Time& time);
};
