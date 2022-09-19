#pragma once
#include "GizmoTransform.h"

class CGizmoTransformScale : public CGizmoTransform
{
public:
	CGizmoTransformScale();
	virtual ~CGizmoTransformScale();

	// return true if gizmo transform capture mouse
	virtual bool OnMouseDown(unsigned int x, unsigned int y);
	virtual void OnMouseMove(unsigned int x, unsigned int y);
	virtual void OnMouseUp(unsigned int x, unsigned int y);

	virtual void Draw();

	virtual void SetSnap(const float snap);
	virtual void SetSnap(float snapx, float snapy, float snapz);

	float GetScaleSnap();

	virtual void ApplyTransform(tvector3& trans, bool bAbsolute);

protected:
	enum SCALETYPE
	{
		SCALE_NONE,
		SCALE_X,
		SCALE_Y,
		SCALE_Z,
		SCALE_XY,
		SCALE_XZ,
		SCALE_YZ,
		SCALE_XYZ
	};
	SCALETYPE m_ScaleType, m_ScaleTypePredict;


	unsigned int m_LockX, m_LockY;
	float m_ScaleSnap;

	bool GetOpType(SCALETYPE &type, unsigned int x, unsigned int y);
	void SnapScale(float &val);
};