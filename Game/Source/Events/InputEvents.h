#pragma once

#include "Source/Events/Event.h"

namespace Firelight::Events::InputEvents
{
	NEW_EVENT(OnPlayerMoveEvent);

	NEW_EVENT(OnPlayerMoveUpEvent);
	NEW_EVENT(OnPlayerMoveUpEventRelease);

	NEW_EVENT(OnPlayerMoveRightEvent);
	NEW_EVENT(OnPlayerMoveRightEventRelease);

	NEW_EVENT(OnPlayerMoveLeftEvent);
	NEW_EVENT(OnPlayerMoveLeftEventRelease);

	NEW_EVENT(OnPlayerMoveDownEvent);
	NEW_EVENT(OnPlayerMoveDownEventRelease);

	NEW_EVENT(OnInteractEvent);
	NEW_EVENT(SpawnItemEvent);
	NEW_EVENT(RemoveHealthEvent);
}