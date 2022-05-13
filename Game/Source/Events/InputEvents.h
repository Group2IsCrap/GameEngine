#pragma once

#include "Source/Events/Event.h"

namespace Firelight::Events::InputEvents
{
	NEW_EVENT(OnPlayerMoveEvent);
	NEW_EVENT(OnPlayerMoveUpEvent);
	NEW_EVENT(OnPlayerMoveRightEvent);
	NEW_EVENT(OnPlayerMoveLeftEvent);
	NEW_EVENT(OnPlayerMoveDownEvent);
	NEW_EVENT(OnInteractEvent);
	NEW_EVENT(SpawnItemEvent);
	NEW_EVENT(RemoveHealthEvent);
	NEW_EVENT(AttackEvent);
}