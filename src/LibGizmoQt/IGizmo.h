#pragma once
#include "Utilities.h"

class IGizmo
{
public:
	enum LOCATION
	{
		LOCATE_VIEW,
		LOCATE_WORLD,
		LOCATE_LOCAL,
	};

	enum ROTATE_AXIS
	{
		AXIS_X = 1,
		AXIS_Y = 2,
		AXIS_Z = 4,
		AXIS_TRACKBALL = 8,
		AXIS_SCREEN = 16,
		AXIS_ALL = 31

	};

	virtual void SetEditMatrix(float *pMatrix) = 0;
	virtual void SetEditMatrix(QMatrix4x4 &pMatrix) = 0;

	virtual void SetCameraMatrix(const float *Model, const float *Proj) = 0;
	virtual void SetScreenDimension(int screenWidth, int screenHeight) = 0;
	virtual void SetDisplayScale(float aScale) = 0;

	// return true if gizmo transform capture mouse
	virtual bool OnMouseDown(unsigned int x, unsigned int y) = 0;
	virtual void OnMouseMove(unsigned int x, unsigned int y) = 0;
	virtual void OnMouseUp(unsigned int x, unsigned int y) = 0;

	// snaping
	virtual void UseSnap(bool bUseSnap) = 0;
	virtual bool IsUsingSnap() = 0;
	virtual void SetSnap(float snapx, float snapy, float snapz) = 0;
	virtual void SetSnap(const float snap) = 0;


	virtual void SetLocation(LOCATION aLocation) = 0;
	virtual LOCATION GetLocation() = 0;
	virtual void SetAxisMask(unsigned int mask) = 0;

	// rendering
	virtual void Draw() = 0;
};

// Create new gizmo
IGizmo *CreateMoveGizmo();
IGizmo *CreateRotateGizmo();
IGizmo *CreateScaleGizmo();