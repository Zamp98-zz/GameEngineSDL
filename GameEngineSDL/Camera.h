#pragma once
#include "SoftwareRenderer.h"
#include "Matrix.h"
#include "ObjLoader.h"
#include <SDL.h>
#include "Vector.h"



typedef struct angle {
	float x;
	float y;
	float z;
}Angle;

float correctAngle(float angle) {
	angle = angle * 100;
	float fact = 360 / 1.75;
	if(angle > 0)
		while (angle > fact)
			angle = angle - fact;
	else if(angle < 0)
		while (angle < -fact)
			angle = angle + fact;

	return angle/100;
}
double min(double a, double b) {
	if (a == b || a < b) return a;
	else return b;
}

float scalarProduct(Vector3d a, Vector3d b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

class Camera {
public:
	int fov;
	Matrix perspective;
	Position pos;
	Angle angle;
	Resolution res;
	Vector3d direction;
	Camera() {
		fov = 90.0;
		setProjectionMatrix(fov, 0.1, 100);
		pos.x = pos.y = pos.z = 0.0;
		angle.x = angle.z = angle.y = 0.0;
		direction.z = 1;
		direction.x = direction.y = direction.w = 0;
	}
	void setResolution(int width, int height) {
		res.height = height;
		res.width = width;
	}
	Position centerScreen() {
		Position p;
		p.x = res.width / 2;
		p.y = res.height / 2;
		return p;
	}
	void resetAngle() {
		//pos.x = pos.y = pos.z = 0.0;
		angle.x = angle.z = angle.y = 0.0;
	}
	void translateX(float s) {
		pos.x += s;
	}
	void translateY(float s) {
		pos.y += s;
	}
	void translateZ(float s) {
		pos.z += s;
	}
	void rotateX(float s) {
		angle.x += s;
		//angle.x = correctAngle(angle.x);
		updateVectorAngle();
	}
	void rotateY(float s) {
		angle.y += s;
		//angle.y = correctAngle(angle.y);
		updateVectorAngle();
	}
	void rotateZ(float s) {
		angle.z += s;
		//angle.z = correctAngle(angle.z);
		updateVectorAngle();
	}
	void setProjectionMatrix(const float& angleOfView, const float& near, const float& far)
	{
		// do some work here
		float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
		perspective.values[0][0] = scale;
		perspective.values[1][1] = scale;
		perspective.values[2][2] = -far / (far - near);
		perspective.values[3][2] = -far * near / (far - near);
		perspective.values[2][3] = -1;
		perspective.values[3][3] = 0;
	}
	Position multPointMatrix(Position in, Matrix M)
	{
		//out = in * Mproj;
		Position out;
		out.x = in.x * M.values[0][0] + in.y * M.values[1][0] + in.z * M.values[2][0] + /* in.z = 1 */ M.values[3][0];
		out.y = in.x * M.values[0][1] + in.y * M.values[1][1] + in.z * M.values[2][1] + /* in.z = 1 */ M.values[3][1];
		out.z = in.x * M.values[0][2] + in.y * M.values[1][2] + in.z * M.values[2][2] + /* in.z = 1 */ M.values[3][2];
		float w = in.x * M.values[0][3] + in.y * M.values[1][3] + in.z * M.values[2][3] + /* in.z = 1 */ M.values[3][3];
		// normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
		if (w != 1) {
			out.x /= w;
			out.y /= w;
			out.z /= w;
		}
		return out;
	}
	DisplayList applyPerspective(DisplayList l) {
		int i;
		//unsigned char* buffer = new unsigned char[SCREEN_WIDTH * SCREEN_HEIGHT];
		int s = l.objects.size();
		Matrix worldToCamera;
		worldToCamera.values[3][1] = -10;
		worldToCamera.values[3][2] = -20;
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			int j;
			int v = o.shape.vertexAmount;
			for (j = 0; j < v; j++) {
				Position original;
				Position p = o.shape.vertexToPosition(j);
				original = p;
				p = multPointMatrix(p, worldToCamera);
				p = multPointMatrix(p, perspective);
				if ((p.x < -1 || p.x > 1) && (p.y < -1 || p.y > 1)) continue;
				p.x = min(SCREEN_WIDTH - 1, (int)((p.x + 1) * 0.5 * SCREEN_WIDTH));
				p.y = min(SCREEN_HEIGHT - 1, (int)((1 - (p.y + 1) * 0.5) * SCREEN_HEIGHT));
				
				o.shape.setVertex(j, p);
			}
			l.objects[i] = o.shape;
		}
		//l = scale(originalScale, l);
		return l;
	}
	DisplayList toCameraCoordinates(DisplayList l) {
		int i;
		int s = l.objects.size();
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			o = translate(o, X, res.width/2);
			o = translate(o, Y, res.height/2);
			l.objects[i] = o.shape;
		}
		return l;
	}
	Position distance(Position a, Position b) {
		Position r;
		r.x = b.x - a.x;
		r.y = b.y - a.y;
		r.z = b.z - a.z;
		return r;
	}
	Object prepareRotation(Object o) {
		float deltaX, deltaY, deltaZ;
		o.pos = objectCenter(o);
		Position dist = distance(o.pos, pos);//distance from camera to the object
		if (dist.x != 0) 
			o = translate(o, X, dist.x);
		if (dist.y != 0)
			o = translate(o, Y, dist.y);
		if (dist.z != 0)
			o = translate(o, Z, dist.z);
		return o;
	}
	Vector3d normalizeVector(Vector3d v) {
		float n = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		v.x = v.x / n;
		v.y = v.y / n;
		v.z = v.z / n;
		return v;
	}
	Position faceCenter(Object o, int i) {
		int v0, v1, v2;
		Vertex* v = o.getVerticesFromFace(i);
		float x = v[0].x + v[1].x + v[2].x;
		float y = v[0].y + v[1].y + v[2].y;
		float z = v[0].z + v[1].z + v[2].z;
		x = x / 3;
		y = y / 3;
		z = z / 3;
		Position p;
		p.x = x;
		p.y = y;
		p.z = z;
		return p;
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
	void updateVectorAngle(){
		Matrix m;
		m.values[0][0] = this->direction.x;
		m.values[0][1] = this->direction.y;
		m.values[0][2] = this->direction.z;
		m.values[0][3] = 0;
		
		m = m.rotateX(m.values, this->angle.x);
		m = m.rotateY(m.values,this->angle.y);
		m = m.rotateZ(m.values,this->angle.z);
		
		this->direction.x = m.values[0][0];
		this->direction.y = m.values[0][1];
		this->direction.z = m.values[0][2];
	}

	void cameraRender(DisplayList l, SDL_Renderer *gRenderer) {
		//TODO deltas and blablabla then render
		int i;
		int s = l.objects.size();
		l = toCameraCoordinates(l);
		bool removed = false;
		vector<bool> hiddenFaces;
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			o = setMeshNormals(o);
			o = prepareRotation(o);
			
			o = rotateObject(o, X, angle.x);
			o = rotateObject(o, Y, angle.y);		
			o = rotateObject(o, Z, angle.z);


			Position dist = distance(o.pos, pos);//distance from camera to the object
			if (dist.z < 1)//if its too close or behind the camera
			{
				l.removeIndex(i);
				printf("do not draw\n");
				removed = true;
			}
			int j, s;
			s = o.shape.faceAmount;
			
			for (j = 0; j < s; j++) {
				Vector3d n;
				n.x = o.shape.MeshNormals[j].x;
				n.y = o.shape.MeshNormals[j].y;
				n.z = o.shape.MeshNormals[j].z;
				Vector3d camera;
				camera.x = this->direction.x;
				camera.y = this->direction.y;
				camera.z = this->direction.z;
				Vector3d cameraToObjectFace;
				Position p = faceCenter(o, j);
				cameraToObjectFace.x = this->pos.x - p.x;
				cameraToObjectFace.y = this->pos.y - p.y;
				cameraToObjectFace.z = this->pos.z - p.z;
				float a = scalarProduct(n, cameraToObjectFace);
				printf("face %d angle %f\n", j, a);
				if (a >= 0) {
					o.shape.hideFace(j);
					hiddenFaces.push_back(true);
					printf("hide face: %d\n", j);
					//s--;
				}
				
				
			}
			if(!removed)
				l.objects[i] = o.shape;
		}
		l = applyPerspective(l);
		
		//renderWireframe(gRenderer, l);
		render(gRenderer, l);
	}
};
