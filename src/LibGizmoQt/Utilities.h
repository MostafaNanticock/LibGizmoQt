#pragma once
#include <qmatrix4x4.h>

#include "ZBaseMaths.h"

QMatrix4x4 toQMatrix4x4(float *pMatrix);

float* toFloat(QMatrix4x4 matrix, float *resultMatrix);

tmatrix toTMatrix(QMatrix4x4 matrix);