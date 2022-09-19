#pragma once
#include "GizmoTransform.h"

class CGizmoTransformRotate : public CGizmoTransform
{

public:
	CGizmoTransformRotate();
	virtual ~CGizmoTransformRotate();

	// return true if gizmo transform capture mouse
	virtual bool OnMouseDown(unsigned int x, unsigned int y);
	virtual void OnMouseMove(unsigned int x, unsigned int y);
	virtual void OnMouseUp(unsigned int x, unsigned int y);

	virtual void Draw();

	virtual void SetSnap(const float snap);
	virtual void SetSnap(float snapx, float snapy, float snapz);

	float GetAngleSnap();

	virtual void ApplyTransform(tvector3& trans, bool bAbsolute);

protected:
	enum ROTATETYPE
	{
		ROTATE_NONE,
		ROTATE_X,
		ROTATE_Y,
		ROTATE_Z,
		ROTATE_SCREEN,
		ROTATE_TWIN
	};

	ROTATETYPE m_RotateType, m_RotateTypePredict;
	tplane m_plan;
	tvector3 m_LockVertex, m_LockVertex2;
	float m_Ng2;
	tvector3 m_Vtx, m_Vty, m_Vtz;
	tvector3 m_Axis, m_Axis2;
	tmatrix m_OrigScale, m_InvOrigScale;
	float m_AngleSnap;


	bool GetOpType(ROTATETYPE &type, unsigned int x, unsigned int y);
	bool CheckRotatePlan(tvector3 &vNorm, float factor, const tvector3 &rayOrig, const tvector3 &rayDir, int id);
	void Rotate1Axe(const tvector3& rayOrigin, const tvector3& rayDir);

};