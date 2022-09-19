#include "Utilities.h"

QMatrix4x4 toQMatrix4x4(float *pMatrix)
{
	QMatrix4x4 result;
	for (int i = 0; i < 16; i++)
		result.data()[i] = pMatrix[i];

	return result;
}

float* toFloat(QMatrix4x4 matrix, float *resultMatrix)
{
	if (resultMatrix)
	{
		matrix.copyDataTo(resultMatrix);
		return resultMatrix;
	}

	return nullptr;
}

tmatrix toTMatrix(QMatrix4x4 matrix)
{
	tmatrix result;

	for (int i = 0; i < 16; i++)
		result[i] = matrix.data()[i];

	return result;
}
