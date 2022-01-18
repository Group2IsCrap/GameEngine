
#pragma once
#include "PhysicsComponents.h"

class CollisionHandler
{

public:
	virtual void Update(double dt) override;
	
	bool CheckBoxOnBoxCollision(PhysicsComponent Obj1, PhysicsComponent Obj2); //checks for collision between two objects using axis aligned bounding boxes

	bool CheckCircleOnCircleCollision(PhysicsComponent Obj1, PhysicsComponent Obj2); // check for collision between two objects using a circle

	bool CheckBoxOnCircleCollision(PhysicsComponent Obj1, PhysicsComponent Obj2); // check for collision between two objects using a circle and a box


};
