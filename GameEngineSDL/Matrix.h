#pragma once
#include <stdlib.h>
#include <stdio.h>


float** alocateMatrix(int i, int j);//dirty allocation of a matrix
float* copyLine(float* a, float* b, int s);//copy the content from A to B
float* copyColumn(float** a, float* b, int line, int columns);
float lineColumn(float** matrixA, float** matrixB, int lA, int cB, int linesA, int columnsA, int linesB, int columnsB);
float** multiplyMatrix(float** a, float** b, const int ai, const int aj, const int bi, const int bj);