#include "TransformData.h"

#include "BasicComponents.h"

void Firelight::ECS::TransformData::SetPosition(const Firelight::Maths::Vec3f& pos)
{
	Firelight::Maths::Vec3f difference = pos - position;

	// Update child objects
	for (auto child : children)
	{
		TransformComponent* transform = child.second->GetComponent<TransformComponent>();
		transform->SetPosition(transform->GetPosition() + difference);
	}

	position = pos;
}

void Firelight::ECS::TransformData::FlipX(bool flip, bool flipPos)
{
	if (isFlipped == flip)
		return;

	// TODO : Binary search to root and flip basd on root. Currently,
	// root must be flipped or items will not work aka do not flip child objects unless it's the
	// top of the chain that you care for.
	for (auto child : children)
	{
		child.second->GetComponent<TransformComponent>()->FlipX(flip);
	}

	if (flipPos)
	{
		Firelight::Maths::Vec3f inverted = Firelight::Maths::Vec3f(position.x *= -1, position.y, position.z);
		SetPosition(inverted);
	}
	isFlipped = flip;
}

void Firelight::ECS::TransformData::SetParent(Entity* parent)
{
	if (children.contains(parent->GetEntityID()))
	{
		return;
	}
	this->parent = parent;
}
