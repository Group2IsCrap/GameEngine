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
                if (entity->GetComponent<PhysicsComponentt>(hasBox))
                {

                }
                CheckBoxOnBoxCollision(entity->GetComponent<TransformComponent>(), )



            }
        }
    }
}

bool CollisionHandler::CheckBoxOnBoxCollision(Firelight::ECS::TransformComponent Obj1, Firelight::ECS::TransformComponent Obj2) //checks for collision between two objects using axis aligned bounding boxes
{

    if (
        Obj1.posX < Obj2.posX + Obj2.width &&
        Obj1.posX + Obj1.width > Obj2.posX &&
        Obj1.posY < Obj2.posY + Obj2.height &&
        Obj1.posY + Obj1.height > Obj2.posY
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionHandler::CheckCircleOnCircleCollision(Firelight::ECS::TransformComponent Obj1, Firelight::ECS::TransformComponent Obj2)
{
    if ((Obj1.posX - Obj2.posX) * (Obj1.posX - Obj2.posX) + (Obj1.posY - Obj2.posY) * (Obj1.posY - Obj2.posY) < Obj1.radius * Obj2.radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//test function - delete later
int main(Firelight::ECS::TransformComponent Obj1, Firelight::ECS::TransformComponent Obj2)
{

    CollisionHandler collisionHandler;
    Obj1.posX = 10;
    Obj1.posY = 10;
    Obj1.height = 10;
    Obj1.width = 10;
    Obj1.radius = 20;

    Obj2.posX = 10;
    Obj2.posY = 10;
    Obj2.height = 10;
    Obj2.width = 10;
    Obj2.radius = 20;

    if (collisionHandler.CheckCircleOnCircleCollision(Obj1, Obj2))
    {
        std::cout << "Circle Collision Detected\n";
    }
    else
    {
        std::cout << "No Circle Collision\n";
    }

    if (collisionHandler.CheckBoxOnBoxCollision(Obj1, Obj2))
    {
        std::cout << "Box Collision Detected\n";
    }
    else
    {
        std::cout << "Box Circle Collision\n";
    }
}

