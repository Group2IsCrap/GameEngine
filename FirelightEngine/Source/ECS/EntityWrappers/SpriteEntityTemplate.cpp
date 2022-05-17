#include "SpriteEntityTemplate.h"

#include "../Components/RenderingComponents.h"

Firelight::ECS::SpriteEntityTemplate::SpriteEntityTemplate() : Firelight::ECS::GameEntityTemplate()
{
	AddComponent<SpriteComponent>();
}
