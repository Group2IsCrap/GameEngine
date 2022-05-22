#include "GameEntityTemplate.h"

#include "../Components/BasicComponents.h"

Firelight::ECS::GameEntityTemplate::GameEntityTemplate() : Template()
{
	AddComponent<IdentificationComponent>()->name = "Game Entity Template";
	AddComponent<StaticComponent>();
	AddComponent<LayerComponent>();
	AddComponent<TransformComponent>();
}
