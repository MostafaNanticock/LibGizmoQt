#pragma once
#include "GizmoTransform.h"

class CGizmoTransformMove : public CGizmoTransform
{

public:
	CGizmoTransformMove();
	virtual ~CGizmoTransformMove();

	// return true if gizmo transform capture mouse
	virtual bool OnMouseDown(unsigned int x, unsigned int y);
	virtual void OnMouseMove(unsigned int x, unsigned int y);
	virtual void OnMouseUp(unsigned int x, unsigned int y);

	virtual void Draw();

	// snap
	virtual void SetSnap(float snapx, float snapy, float snapz);
	virtual void SetSnap(const float snap) {}

	tvector3 GetMoveSnap();

	virtual void ApplyTransform(tvector3& trans, bool bAbsolute);

protected:
	enum MOVETYPE
	{
		MOVE_NONE,
		MOVE_X,
		MOVE_Y,
		MOVE_Z,
		MOVE_XY,
		MOVE_XZ,
		MOVE_YZ,
		MOVE_XYZ
	};
	MOVETYPE m_MoveType, m_MoveTypePredict;
	tvector3 m_MoveSnap;

	bool GetOpType(MOVETYPE &type, unsigned int x, unsigned int y);
	tvector3 RayTrace(tvector3& rayOrigin, tvector3& rayDir, tvector3& norm);
};