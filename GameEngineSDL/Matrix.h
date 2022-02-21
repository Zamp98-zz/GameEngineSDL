#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define DIMENSION 4




typedef struct vector3d {
	float x;
	float y;
	float z;
	float w;
}Vector3f;

class Matrix {
public:
	float values[DIMENSION][DIMENSION];
	float ** copy(float** a, int l, int k) {
		int i;
		for (i = 0; i < l; i++) {
			int j;
			for (j = 0; j < k; j++)
				a[i][j] = values[i][j];
		}
		return a;
	}
	Matrix() {
		int i, j;
		for (i = 0; i < DIMENSION; i++)
			for (j = 0; j < DIMENSION; j++)
				if (i != j)
					values[i][j] = 0;
				else
					values[i][j] = 1;
	}
};
Matrix scaleMatrix(float a[DIMENSION][DIMENSION], float value);
Matrix addMatrix(float a[DIMENSION][DIMENSION], float b[DIMENSION][DIMENSION]);
Matrix multiplyMatrix(float a[DIMENSION][DIMENSION], float b[DIMENSION][DIMENSION]);

Matrix rotateX(float a[DIMENSION][DIMENSION], float angle);
Matrix rotateY(float a[DIMENSION][DIMENSION], float angle);
Matrix rotateZ(float a[DIMENSION][DIMENSION], float angle);

Matrix translateX(float a[DIMENSION][DIMENSION], float delta);
Matrix translateY(float a[DIMENSION][DIMENSION], float delta);
Matrix translateZ(float a[DIMENSION][DIMENSION], float delta);