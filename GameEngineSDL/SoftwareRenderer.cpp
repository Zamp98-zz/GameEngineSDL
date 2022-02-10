#include "SoftwareRenderer.h"
#include <math.h>


float M[4][4];

void setProjectionMatrix(const float& angleOfView, const float& near, const float& far)
{
	// set the basic projection matrix
	float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
	M[0][0] = scale; // scale the x coordinates of the projected point 
	M[1][1] = scale; // scale the y coordinates of the projected point 
	M[2][2] = -far / (far - near); // used to remap z to [0,1] 
	M[3][2] = -far * near / (far - near); // used to remap z [0,1] 
	M[2][3] = -1; // set w = -z 
	M[3][3] = 0;
}
/*
a = M[1][3]
b = M[3][3]
a*b = M[1][3]
[1,2,3][1,1,1]
       [1,1,1]
	   [1,1,1]

*/
float** alocateMatrix(int i, int j){//dirty allocation of a matrix
	float** matrix = (float**)malloc(sizeof(float*) * i);
	int a;
	for (a = 0; a < i; a++) {
		matrix[a] = (float*)(malloc(sizeof(float) * j));
	}
	return matrix;
}

float* copyLine(float *a, float *b, int s) {//copy the content from A to B
	int i;
	for (i = 0; i < s; i++)
		b[i] = a[i];
	return b;
}
float* copyColumn(float ** a, float *b, int line, int columns) {
	int j;
	for (j = 0; j < columns; j++)
		b[j] = a[line][j];
	return b;
}

float lineColumn(float **matrixA, float **matrixB, int lA, int cB, int linesA, int columnsA, int linesB, int columnsB) {
	float* lineA = (float*)(malloc(sizeof(float)*columnsA));
	lineA = copyLine(matrixA[lA], lineA, columnsA);
	float * columnB = (float*)(malloc(sizeof(float)*linesB));
	// the size of the A line is equal to the B column

}

void multiplyMatrix(float **a, float **b,const int ai,const int aj,const int bi,const int bj){
	float **result = alocateMatrix(ai, bj);
	//multiply the lines of the first matrix with de collunms of the second
	int i;
	for (i = 0; i < ai; i++) {
		int j;
		for (j = 0; j < bj; j++) {
			result[i][j] = lineColumn(a, b, i, j, ai, aj, bi, bj);
		}
	}
}

void renderWireframe(SDL_Renderer* gRenderer, DisplayList l) {
	
}
DisplayList applyPerspective(DisplayList l, Perspective p) {
	int i;
	for (i = 0; i < l.objects.size(); i++) {
		
		int j;
		for (j = 0; j < l.objects[i].Vertices.size(); j++) {
			Position original;
			original.x = l.objects[i].Vertices[j].x;
			original.y = l.objects[i].Vertices[j].y;
			original.z = l.objects[i].Vertices[j].z;
			l.objects[i].Vertices[j].x = original.x * p.z / (p.z + original.z);
			l.objects[i].Vertices[j].y = original.y * p.z / (p.z + original.z);
		}
		
	}
	return l;
}

