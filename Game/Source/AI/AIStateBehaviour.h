#pragma once

#include <Source/Utils/Time.h>

class AIStateBehaviour
{
public:
	AIStateBehaviour(class AIEntity* aiEntity);

	virtual void Update(const Firelight::Utils::Time& time);

private:
	class AIEntity* m_AIEntity;
};
