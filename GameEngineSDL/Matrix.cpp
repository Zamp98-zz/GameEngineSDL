#include "Matrix.h"

float indentity[DIMENSION][DIMENSION] = { {1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1} };

Matrix multiplyMatrix(float a[DIMENSION][DIMENSION], float b[DIMENSION][DIMENSION]) {
	//multiply the lines of the first matrix with de collunms of the second
	int i, j, k;
	int r, c;
	c = r = 4;
	Matrix ret;
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
	float aux[DIMENSION][DIMENSION] = { {value, 0, 0, 0},{0, value, 0, 0},{0, 0, value, 0},{0, 0, 0, value} };
	Matrix r = multiplyMatrix(a, aux);
	return r;
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