#pragma once
#include <cstdlib>

class CombatCalculations
{
public:
	float CalculateTriangleArea(int x1, int y1, int x2, int y2, int x3, int y3);

	bool IsPointInTriangle(int PointOneX, int PointOneY, int PointTwoX, int PointTwoY, int PointThreeX, int PointThreeY, int PointToCheckX, int PointToCheckY);
	
};

