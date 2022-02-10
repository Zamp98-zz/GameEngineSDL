#include "Matrix.h"

float ** alocateMatrix(int i, int j) {//dirty allocation of a matrix
	float** matrix = (float**)malloc(sizeof(float*) * i);
	int a;
	for (a = 0; a < i; a++) {
		matrix[a] = (float*)(malloc(sizeof(float) * j));
	}
	return matrix;
}

float * copyLine(float* a, float* b, int s) {//copy the content from A to B
	int i;
	for (i = 0; i < s; i++)
		b[i] = a[i];
	return b;
}
float * copyColumn(float** a, float* b, int line, int columns) {
	int j;
	for (j = 0; j < columns; j++)
		b[j] = a[line][j];
	return b;
}

float lineColumn(float** matrixA, float** matrixB, int lA, int cB, int linesA, int columnsA, int linesB, int columnsB) {
	float* lineA = (float*)(malloc(sizeof(float) * columnsA));
	lineA = copyLine(matrixA[lA], lineA, columnsA);
	float* columnB = (float*)(malloc(sizeof(float) * linesB));
	columnB = copyColumn(matrixA, columnB, cB, columnsB);
	int i;
	float r = .0;
	// the size of the A line is equal to the B column
	for (i = 0; i < columnsB; i++) {
		r += lineA[i] * columnB[i];
	}
	return r;
}

float** multiplyMatrix(float** a, float** b, const int ai, const int aj, const int bi, const int bj) {
	float** result = alocateMatrix(ai, bj);
	//multiply the lines of the first matrix with de collunms of the second
	int i;
	for (i = 0; i < ai; i++) {
		int j;
		for (j = 0; j < bj; j++) {
			result[i][j] = lineColumn(a, b, i, j, ai, aj, bi, bj);
		}
	}
	return result;
}