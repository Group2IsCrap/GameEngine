#include "GameEntityTemplate.h"

#include "../Components/BasicComponents.h"

Firelight::ECS::GameEntityTemplate::GameEntityTemplate() : Template()
{
	AddComponent<IdentificationComponent>();
	AddComponent<StaticComponent>();
	AddComponent<LayerComponent>();
	AddComponent<TransformComponent>();
}
