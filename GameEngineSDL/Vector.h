#pragma once
#include "Matrix.h"
class Vector3d {
public:
	float x;
	float y;
	float z;
	float w;
	Vector3d() {
		x = y = z = w = 0;
	}

};

Vector3d crossProduct(Vector3d vect_A, Vector3d vect_B){
	Vector3d cross_P;
	cross_P.x = vect_A.y * vect_B.z - vect_A.z * vect_B.y;
	cross_P.y = vect_A.z * vect_B.x - vect_A.x * vect_B.z;
	cross_P.z = vect_A.x * vect_B.y - vect_A.y * vect_B.x;
	return cross_P;
}
