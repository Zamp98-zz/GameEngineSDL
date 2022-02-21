#pragma once
#include "Matrix.h"
class Position {
public:
	float x;
	float y;
	float z;
	float w;
	Position() {
		x = y = z = w = 0;
	}
	Position toPosition(Matrix m) {
		Position temp;
		temp.x = m.values[0][0];
		temp.y = m.values[0][1];
		temp.z = m.values[0][2];
		temp.w = m.values[0][3];
		return temp;
	}
	Position columnToPosition(Matrix m) {
		Position temp;
		temp.x = m.values[0][0];
		temp.y = m.values[1][0];
		temp.z = m.values[2][0];
		temp.w = m.values[3][0];
		return temp;
	}
	Matrix toMatrixColumn(Position p) {
		Matrix m;
		m.values[0][0] = p.x;
		m.values[0][1] = p.y;
		m.values[0][2] = p.z;
		m.values[0][3] = p.w;
		return m;
	}

};