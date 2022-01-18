
#pragma once
#include "PhysicsComponents.h"

class CollisionHandler
{

public:
	virtual void Update(double dt) override;

	bool CheckBoxOnBoxCollision(Firelight::ECS::TransformComponent Obj1, Firelight::ECS::TransformComponent Obj2); //checks for collision between two objects using axis aligned bounding boxes

	bool CheckCircleOnCircleCollision(Firelight::ECS::TransformComponent Obj1, Firelight::ECS::TransformComponent Obj2); // check for collision between two objects using a circle

};
