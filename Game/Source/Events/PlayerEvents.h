#pragma once

#include "Source/Events/Event.h"

namespace Firelight::Events::PlayerEvents
{
	NEW_EVENT(OnPlayerHealthChangedEvent);
	NEW_EVENT(OnPlayerDiedEvent);
	NEW_EVENT(OnPlayerRespawnEvent);
	NEW_EVENT(AddHealth);
	NEW_EVENT(ChangeWeapon);
	NEW_EVENT(ChangeHat);
	NEW_EVENT(ChangeArmour);
}