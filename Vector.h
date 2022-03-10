#pragma once
#include "Matrix.h"

typedef struct angle {
	float x;
	float y;
	float z;
}Angle;
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

Vector3d rotateVector(Angle angle, Vector3d v) {
	Matrix m;
	m.values[0][0] = v.x;
	m.values[0][1] = v.y;
	m.values[0][2] = v.z;
	m.values[0][3] = 0;

	m = m.rotateX(m.values, angle.x);
	m = m.rotateY(m.values, angle.y);
	m = m.rotateZ(m.values, angle.z);
	v.x = m.values[0][0];
	v.y = m.values[0][1];
	v.z = m.values[0][2];
	return v;
}

Vector3d crossProduct(Vector3d vect_A, Vector3d vect_B){
	Vector3d cross_P;
	cross_P.x = vect_A.y * vect_B.z - vect_A.z * vect_B.y;
	cross_P.y = vect_A.z * vect_B.x - vect_A.x * vect_B.z;
	cross_P.z = vect_A.x * vect_B.y - vect_A.y * vect_B.x;
	return cross_P;
}
Vector3d normalizeVector(Vector3d v) {
	float n = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x = v.x / n;
	v.y = v.y / n;
	v.z = v.z / n;
	return v;
}
Object setMeshNormals(Object o) {
	int i;
	for (i = 0; i < o.shape.faceAmount; i++) {
		Vector3d a, b, cross;
		int v0, v1, v2;
		v0 = o.shape.Faces[i].Vertices[0] - 1;
		v1 = o.shape.Faces[i].Vertices[1] - 1;
		v2 = o.shape.Faces[i].Vertices[2] - 1;

		a.x = o.shape.Vertices[v1].x - o.shape.Vertices[v0].x;
		a.y = o.shape.Vertices[v1].y - o.shape.Vertices[v0].y;
		a.z = o.shape.Vertices[v1].z - o.shape.Vertices[v0].z;

		b.x = o.shape.Vertices[v2].x - o.shape.Vertices[v0].x;
		b.y = o.shape.Vertices[v2].y - o.shape.Vertices[v0].y;
		b.z = o.shape.Vertices[v2].z - o.shape.Vertices[v0].z;

		cross = crossProduct(a, b);
		cross = normalizeVector(cross);
		o.shape.insertMeshNormal(cross.x, cross.y, cross.z);
	}
	return o;
}
Vector3d sumVectors(Vector3d a, Vector3d b) {
	Vector3d r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	return r;
}