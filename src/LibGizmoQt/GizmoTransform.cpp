#include "GizmoTransform.h"

inline CGizmoTransform::CGizmoTransform()
{
	m_pMatrix = nullptr;
	m_bUseSnap = false;
	//mCamera = NULL;
	//mTransform = NULL;
	mEditPos = mEditScale = nullptr;
	mEditQuaternion = nullptr;
	mMask = AXIS_ALL;
	m_Lng = 1.f;
	mScreenHeight = mScreenWidth = 1;
	m_ScreenFactor = 1;
	mDisplayScale = 1.f;
}

inline CGizmoTransform::~CGizmoTransform()
{
}

inline void CGizmoTransform::SetEditMatrix(float * pMatrix)
{
	m_pMatrix = (tmatrix*)pMatrix;
	//mTransform = NULL;

	mEditPos = mEditScale = NULL;
	mEditQuaternion = NULL;
}

inline void CGizmoTransform::SetScreenDimension(int screenWidth, int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
}

inline void CGizmoTransform::SetCameraMatrix(const float * Model, const float * Proj)
{
	m_Model = *(tmatrix*)Model;
	m_Proj = *(tmatrix*)Proj;

	m_invmodel = m_Model;
	m_invmodel.Inverse();

	m_invproj = m_Proj;
	m_invproj.Inverse();

	m_CamSrc = m_invmodel.V4.position;
	m_CamDir = m_invmodel.V4.dir;
	m_CamUp = m_invmodel.V4.up;
}

// tools

inline void CGizmoTransform::BuildRay(int x, int y, tvector3 & rayOrigin, tvector3 & rayDir)
{
	float frameX = (float)mScreenWidth;
	float frameY = (float)mScreenHeight;
	tvector3 screen_space;

	// device space to normalized screen space
	screen_space.x = (((2.f * (float)x) / frameX) - 1) / m_Proj.m[0][0];//.right.x;
	screen_space.y = -(((2.f * (float)y) / frameY) - 1) / m_Proj.m[1][1];
	screen_space.z = -1.f;

	// screen space to world space

	rayOrigin = m_invmodel.V4.position;
	rayDir.TransformVector(screen_space, m_invmodel);
	rayDir.Normalize();
}

inline tvector3 CGizmoTransform::GetVector(int vtID)
{
	switch (vtID)
	{
	case 0: return tvector3(1, 0, 0);
	case 1: return tvector3(0, 1, 0);
	case 2: return tvector3(0, 0, 1);
	}
	return tvector3(0, 0, 0);
}

inline tvector3 CGizmoTransform::GetTransformedVector(int vtID)
{
	tvector3 vt;
	switch (vtID)
	{
	case 0: vt = tvector3(1, 0, 0); break;
	case 1: vt = tvector3(0, 1, 0); break;
	case 2: vt = tvector3(0, 0, 1); break;
	}
	if (mLocation == LOCATE_LOCAL)
	{
		vt.TransformVector(*m_pMatrix);
		vt.Normalize();
	}
	return vt;
}

inline void CGizmoTransform::SetAxisMask(unsigned int mask)
{
	mMask = mask;
}

inline void CGizmoTransform::ComputeScreenFactor()
{
	tmatrix viewproj = m_Model * m_Proj;

	tvector4 trf = vector4(m_pMatrix->V4.position.x, m_pMatrix->V4.position.y, m_pMatrix->V4.position.z, 1.f);
	trf.Transform(viewproj);
	m_ScreenFactor = mDisplayScale * 0.15f * trf.w;
}

inline tvector3 CGizmoTransform::RayTrace2(const tvector3 & rayOrigin, const tvector3 & rayDir, const tvector3 & norm, const tmatrix & mt, tvector3 trss, bool lockVTNorm)
{
	extern tvector3 ptd;

	tvector3 df, inters;

	m_plan = vector4(m_pMatrix->GetTranslation(), norm);
	m_plan.RayInter(inters, rayOrigin, rayDir);
	df.TransformPoint(inters, mt);

	df /= GetScreenFactor();
	m_LockVertex = df;
	if (lockVTNorm)
	{
		m_LockVertex.Normalize();
	}
	else
	{
		m_LockVertex = inters;
	}
	m_Lng = df.Length();

	return df;
}

inline float CGizmoTransform::GetScreenFactor()
{
	return m_ScreenFactor;
}

// snap

inline void CGizmoTransform::UseSnap(bool bUseSnap)
{
	m_bUseSnap = bUseSnap;
}

inline bool CGizmoTransform::IsUsingSnap()
{
	return m_bUseSnap;
}

inline void CGizmoTransform::SnapIt(float & pos, float & snap)
{
	float sn = (float)fmod(pos, snap);
	if (fabs(sn) < (snap*0.25f))
		pos -= sn;
	if (fabs(sn) > (snap*0.75f))
		pos = ((pos - sn) + ((sn > 0) ? snap : -snap));
}
