#pragma once

#include "../Events/Event.h"
namespace Firelight::Events::ECS
{
	NEW_EVENT(OnEntityCreatedEvent);
	NEW_EVENT(OnEntityDestroyedEvent);
	NEW_EVENT(OnComponentRegisteredEvent);
	NEW_EVENT(OnComponentAddedEvent);
	NEW_EVENT(OnComponentRemovedEvent);
}