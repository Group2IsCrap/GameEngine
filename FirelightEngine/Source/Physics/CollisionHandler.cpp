#include "CollisionHandler.h"
#include <iostream>

void CollisionHandler::Update(double dt)
{
    for (auto entity : m_entities)
    {
        for (auto entity2 : m_entities)
        {
            if (entity != entity2)
            {
                if (entity->GetComponent<PhysicsComponent>(IsCollidable) && entity2->GetComponent<PhysicsComponent>(IsCollidable))
                {
                    if (entity->GetComponent<PhysicsComponent>(hasBoxCollision) && entity2->GetComponent<PhysicsComponent>(hasBoxCollision))
                    {
                        if (CheckBoxOnBoxCollision(entity, entity2))
                        {
                            entity->GetComponent<PhysicsComponent>(velX) = 0;
                            entity->GetComponent<PhysicsComponent>(velY) = 0;
                        }
                    }
                    else if (entity->GetComponent<PhysicsComponent>(hasCircleCollision) && entity2->GetComponent<PhysicsComponent>(hasCircleCollision))
                    {
                        if (CheckCircleOnCircleCollision(entity, entity2))
                        {
                            entity->GetComponent<PhysicsComponent>(velX) = 0;
                            entity->GetComponent<PhysicsComponent>(velY) = 0;
                        }
                    }
                    else if (entity->GetComponent<PhysicsComponent>(hasBoxCollision) && entity2->GetComponent<PhysicsComponent>(hasCircleCollision))
                    {
                        if (CheckBoxOnCircleCollision(entity, entity2))
                        {
                            entity->GetComponent<PhysicsComponent>(velX) = 0;
                            entity->GetComponent<PhysicsComponent>(velY) = 0;
                        }
                    }
                }
            }
        }
    }
}

bool CollisionHandler::CheckBoxOnBoxCollision(PhysicsComponent Obj1, PhysicsComponent Obj2) //checks for collision between two objects using axis aligned bounding boxes
{
    
    if (
        Obj1.posX < Obj2.posX + Obj2.Width &&
        Obj1.posX + Obj1.Width > Obj2.posX &&
        Obj1.posY < Obj2.posY + Obj2.Height &&
        Obj1.posY + Obj1.Height > Obj2.posY
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionHandler::CheckCircleOnCircleCollision(PhysicsComponent Obj1, PhysicsComponent Obj2) // check for collision between two objects using a circle

{
    if ((Obj1.posX - Obj2.posX) * (Obj1.posX - Obj2.posX) + (Obj1.posY - Obj2.posY) * (Obj1.posY - Obj2.posY) < Obj1.Radius * Obj2.Radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionHandler::CheckBoxOnCircleCollision(PhysicsComponent Obj1, PhysicsComponent Obj2) // check for collision between two objects using a circle and a box

{
    //do this
}



