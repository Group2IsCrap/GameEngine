#include "CombatCalculations.h"

float CombatCalculations::CalculateTriangleArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool CombatCalculations::IsPointInTriangle(int PointOneX, int PointOneY, int PointTwoX, int PointTwoY, int PointThreeX, int PointThreeY, int PointToCheckX, int PointToCheckY)
{
    float A = CalculateTriangleArea(PointOneX, PointOneY, PointTwoX, PointOneY, PointThreeX, PointThreeY);
    float A1 = CalculateTriangleArea(PointToCheckX, PointToCheckY, PointTwoX, PointOneY, PointThreeX, PointThreeY);
    float A2 = CalculateTriangleArea(PointOneX, PointOneY, PointToCheckX, PointToCheckY, PointThreeX, PointThreeY);
    float A3 = CalculateTriangleArea(PointOneX, PointOneY, PointTwoX, PointOneY, PointToCheckX, PointToCheckY);

    return (A == A1 + A2 + A3);
}

//physics helpers
//Overlap circle
