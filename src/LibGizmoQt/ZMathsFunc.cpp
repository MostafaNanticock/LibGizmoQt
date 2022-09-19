#include "stdafx.h"
#include "ZMathsFunc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

float MathFloatBezierCubic(const float &v1, const float &v2, const float& v3, const float& v4, const float s)
{
	float x = v1 * MathBezierCubicSplineF4(s) + v2 * MathBezierCubicSplineF3(s) + v4 * MathBezierCubicSplineF2(s) + v3 * MathBezierCubicSplineF1(s);
	return x;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
