#pragma once
#include "SoftwareRenderer.h"
#include "Matrix.h"
#include "ObjLoader.h"
#include <SDL.h>
#include "Vector.h"






double min(double a, double b) {
	if (a == b || a < b) return a;
	else return b;
}

float scalarProduct(Vector3d a, Vector3d b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z))/RADIAN;
}

class Camera {
public:
	int fov;
	Matrix perspective;
	Position pos;
	Angle angle;
	Resolution res;
	Vector3d frontDirection;
	Vector3d sideDirection;
	Vector3d upDirection;
	Camera() {
		fov = 90.0;
		setProjectionMatrix(fov, 0.1, 100);
		pos.x = pos.y = pos.z = 0.0;
		angle.x = angle.z = angle.y = 0.0;

		frontDirection.z = 1;
		frontDirection.x = frontDirection.y = frontDirection.w = 0;

		sideDirection.x = 1;
		sideDirection.z = sideDirection.y = sideDirection.w = 0;

		upDirection.y = 1;
		upDirection.x = upDirection.z = upDirection.w = 0;
	}
	void setResolution(int width, int height) {
		res.height = height;
		res.width = width;
	}
	Position translateOnVector(float s, Vector3d v) {
		pos.x += s * v.x;
		pos.y += s * v.y;
		pos.z += s * v.z;
		return pos;
	}
	Position centerScreen() {
		Position p;
		p.x = res.width / 2;
		p.y = res.height / 2;
		return p;
	}
	void resetDirections() {
		this->upDirection.y = 1;
		this->upDirection.x = this->upDirection.z = this->upDirection.w = 0;

		this->sideDirection.x = 1;
		this->sideDirection.y = this->sideDirection.z = this->sideDirection.w = 0;

		this->frontDirection.z = 1;
		this->frontDirection.x = this->frontDirection.y = this->frontDirection.w = 0;
	}
	void resetAngle() {
		//pos.x = pos.y = pos.z = 0.0;
		angle.x = angle.z = angle.y = 0.0;
	}
	void translateX(float s) {
		//in x is side translation
		pos = translateOnVector(s, this->sideDirection);
	}
	void translateY(float s) {
		//in y is up translation
		pos = translateOnVector(s, this->upDirection);
	}
	void translateZ(float s) {
		//in y is front translation
		pos = translateOnVector(s, this->frontDirection);
	}
	void rotateX(float s) {
		s = s / RADIAN;
		angle.x += s;
		//angle.x = correctAngle(angle.x);
		resetDirections();
		updateVectorAngle();
	}
	void rotateY(float s) {
		s = s / RADIAN;
		angle.y += s;
		//angle.y = correctAngle(angle.y);
		resetDirections();
		updateVectorAngle();
	}
	void rotateZ(float s) {
		s = s / RADIAN;
		angle.z += s;
		//angle.z = correctAngle(angle.z);
		resetDirections();
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
				p.y = min(SCREEN_HEIGHT - 1, (int)((1-(p.y + 1) * 0.5) * SCREEN_HEIGHT));
				p.z = original.z;
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
			o = translate(o, Z, o.pos.z - this->pos.z);
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
	Object prepareAndRotate(Object o) {
		float deltaX, deltaY, deltaZ;
		o.pos = objectCenter(o);
		Position dist = distance(o.pos, pos);//distance from camera to the object
		if (dist.x != 0) 
			o = translate(o, X, dist.x);
		if (dist.y != 0)
			o = translate(o, Y, dist.y);
		if (dist.z != 0)
			o = translate(o, Z, dist.z);
		o = rotateObject(o, X, this->angle.x);
		o = rotateObject(o, Y, this->angle.y);
		o = rotateObject(o, Z, this->angle.z);
		return o;
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

	void updateVectorAngle(){
		//rotate the 3 vectors
		Angle a = this->angle;
		a.x = -this->angle.x; a.y = -this->angle.y; a.z = -this->angle.z;
		this->frontDirection = rotateVector(a, this->frontDirection);
		this->sideDirection = rotateVector(a, this->sideDirection);
		this->upDirection = rotateVector(a, this->upDirection);
	}

	void cameraRender(DisplayList l, SDL_Renderer *gRenderer) {
		//TODO deltas and blablabla then render
		int i;
		int s = l.objects.size();
		l = toCameraCoordinates(l);
		bool removed = false;
		//vector<bool> hiddenFaces;
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			o = setMeshNormals(o);
			o = prepareAndRotate(o);
			
			//Position dist = distance(o.pos, pos);//distance from camera to the object
			/*if (dist.z < 1)//if its too close or behind the camera
			{
				l.removeIndex(i);
				printf("do not draw\n");
				removed = true;
			}*/
			Position dist = objectCenter(o);
			Vector3d camera, object;
			int j, s;
			s = o.shape.faceAmount;
			camera.x = this->frontDirection.x; camera.y = this->frontDirection.y; camera.z = this->frontDirection.z;
			object.x = dist.x; object.y = dist.y; object.z = dist.z;


			for (j = 0; j < s; j++) {
				Vector3d n;
				
				n.x = o.shape.MeshNormals[j].x;
				n.y = o.shape.MeshNormals[j].y;
				n.z = o.shape.MeshNormals[j].z;
				
				Vector3d cameraToObjectFace;
				Position p = faceCenter(o, j);
				cameraToObjectFace.x = this->pos.x - p.x;
				cameraToObjectFace.y = this->pos.y - p.y;
				cameraToObjectFace.z = this->pos.z - p.z;
				float a = scalarProduct(n, camera);
				printf("face %d angle %f\n", j, a);
				/*printf("camera vectors: front=200(%f, %f, %f), side=200(%f, %f, %f), up=200(%f, %f, %f)\n",
					this->frontDirection.x, this->frontDirection.y, this->frontDirection.z,
					this->sideDirection.x, this->sideDirection.y, this->sideDirection.z,
					this->upDirection.x, this->upDirection.y, this->upDirection.z);*/
				Vector3d r = normalizeVector(cameraToObjectFace);
				r = sumVectors(n, r);
				if ((a >= 0)&&!(r.z == 0)) {
					o.shape.hideFace(j);
					//hiddenFaces.push_back(true);
					printf("hide face: %d\n", j);
					//s--;
				}
			}
			if(!removed)
				l.objects[i] = o.shape;
		}
		l = applyPerspective(l);
		
		renderWireframe(gRenderer, l);
		render(gRenderer, l);
	}
};
