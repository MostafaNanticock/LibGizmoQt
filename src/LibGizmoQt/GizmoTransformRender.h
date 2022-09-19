#pragma once
#include "LibBase.h"

typedef tvector4 tplane;

class CGizmoTransformRender
{
public:
	CGizmoTransformRender() {}
	virtual ~CGizmoTransformRender() {}

	static void DrawCircle(const tvector3 &orig, float r, float g, float b, const tvector3 &vtx, const tvector3 &vty);
	static void DrawCircleHalf(const tvector3 &orig, float r, float g, float b, const tvector3 &vtx, const tvector3 &vty, tplane &camPlan);
	static void DrawAxis(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx, const tvector3 &vty, float fct, float fct2, const tvector4 &col);
	static void DrawCamem(const tvector3& orig, const tvector3& vtx, const tvector3& vty, float ng);
	static void DrawQuad(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3 &axisV);
	static void DrawTri(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3& axisV);
};