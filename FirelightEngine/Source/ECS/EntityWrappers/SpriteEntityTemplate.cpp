#include "SpriteEntityTemplate.h"

#include "../Components/BasicComponents.h"
#include "../Components/RenderingComponents.h"

Firelight::ECS::SpriteEntityTemplate::SpriteEntityTemplate() : Firelight::ECS::GameEntityTemplate()
{
	AddComponent<SpriteComponent>();
	this->GetComponent<IdentificationComponent>()->name = "Sprite Template";
}

Firelight::ECS::SpriteEntityTemplate::SpriteEntityTemplate(std::string name) : Firelight::ECS::SpriteEntityTemplate()
{
	this->GetComponent<IdentificationComponent>()->name = name;
}
