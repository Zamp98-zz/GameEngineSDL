#include "Matrix.h"

float indentity[DIMENSION][DIMENSION] = { {1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1} };


Matrix rotateZ(float a[DIMENSION][DIMENSION], float angle) {
	float c = cos(angle);//avoid recalculation
	float s = sin(angle);
	float rotationMatrixZ[DIMENSION][DIMENSION] =
	{	{c, -s,0, 0},
		{s, c, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1} };
	return multiplyMatrix(a, rotationMatrixZ);
}

Matrix rotateX(float a[DIMENSION][DIMENSION], float angle) {
	float c = cos(angle);//avoid recalculation
	float s = sin(angle);
	float rotationMatrixX[DIMENSION][DIMENSION] =
	{	{1, 0, 0, 0},
		{0, c, -s,0},
		{0, s, c, 0},
		{0, 0, 0, 1} };
	return multiplyMatrix(a, rotationMatrixX);
}
Matrix rotateY(float a[DIMENSION][DIMENSION], float angle) {
	float c = cos(angle);//avoid recalculation
	float s = sin(angle);
	float rotationMatrixY[DIMENSION][DIMENSION] =
	{	{c, 0, s, 0},
		{0, 1, 0,0},
		{-s,0, c, 0},
		{0, 0, 0, 1} };
	return multiplyMatrix(a, rotationMatrixY);
}

Matrix translateX(float a[DIMENSION][DIMENSION], float d) {
	float y = a[0][1];
	float translationMatrixX[DIMENSION][DIMENSION] =
	{	{1, 0, 0, d},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1} 
	};
	return multiplyMatrix(translationMatrixX, a);
}
Matrix translateY(float a[DIMENSION][DIMENSION], float d) {
	
	float translationMatrixY[DIMENSION][DIMENSION] =
	{	{1, 0, 0, 0},
		{0, 1, 0, d},
		{0, 0, 1, 0},
		{0, 0, 0, 1} 
	};
	return multiplyMatrix(translationMatrixY, a);
}
Matrix translateZ(float a[DIMENSION][DIMENSION], float d) {
	float translationMatrixZ[DIMENSION][DIMENSION] =
	{	{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, d},
		{0, 0, 0, 1} 
	};
	return multiplyMatrix(translationMatrixZ, a);
}

Matrix multiplyMatrix(float a[DIMENSION][DIMENSION], float b[DIMENSION][DIMENSION]) {
	//multiply the lines of the first matrix with de collunms of the second
	int i, j, k;
	int r, c;
	c = r = 4;
	Matrix ret;
	ret.init();
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			ret.values[i][j] = 0;
			for (k = 0; k < c; k++)
			{
				ret.values[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return ret;
}

Matrix scaleMatrix(float a[DIMENSION][DIMENSION], float value) {
	float aux[DIMENSION][DIMENSION] = 
	{	{value, 0, 0, 0},
		{0, value, 0, 0},
		{0, 0, value, 0},
		{0, 0, 0, value}
	};
	return multiplyMatrix(a, aux);
}

Matrix addMatrix(float a[DIMENSION][DIMENSION], float b[DIMENSION][DIMENSION]) {
	int i, j, k;
	int r, c;
	c = r = 4;
	Matrix ret;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			ret.values[i][j] = a[i][j] + b[i][j];
		}
	}
	return ret;
}