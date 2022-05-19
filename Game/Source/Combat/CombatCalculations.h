#pragma once
#include <cstdlib>
#include <Source/Maths/Vec3.h>
#include <Source/Physics/PhysicsHelpers.h>
#include "../Player/PlayerComponent.h"
#include "../Core/Layers.h"

using namespace Firelight::Maths;
using namespace Firelight::Physics;



class CombatCalculations
{
public:
	float CalculateTriangleArea(int x1, int y1, int x2, int y2, int x3, int y3);

	bool IsPointInTriangle(int PointOneX, int PointOneY, int PointTwoX, int PointTwoY, int PointThreeX, int PointThreeY, int PointToCheckX, int PointToCheckY);

	static void PlaceSphere(Facing dir, Vec3f nextPos);
	
};