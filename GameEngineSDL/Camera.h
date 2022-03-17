#pragma once
#include "SoftwareRenderer.h"
#include "Matrix.h"
#include "ObjLoader.h"
#include <SDL.h>
#include "Vector.h"


double Z0 = (SCREEN_WIDTH / 2.0) / tan((90 / 2.0) * 3.14159265 / 180.0);

/*Point3D EngineBase::ApplyPerspective(Point3D original)
{
	Point3D toReturn;
	toReturn.x = original.x * Z0 / (Z0 + original.z);
	toReturn.y = original.y * Z0 / (Z0 + original.z);
	toReturn.z = original.z;

	toReturn.u = original.u * Z0 / (Z0 + original.z);
	toReturn.v = original.v * Z0 / (Z0 + original.z);
	toReturn.w = original.w * Z0 / (Z0 + original.z);

	return toReturn;
}*/


double min(double a, double b) {
	if (a == b || a < b) return a;
	else return b;
}

float scalarProduct(Vector3d a, Vector3d b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)) / RADIAN;
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
				p.y = min(SCREEN_HEIGHT - 1, (int)((1 - (p.y + 1) * 0.5) * SCREEN_HEIGHT));
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
			o = translate(o, X, res.width / 2);
			o = translate(o, Y, res.height / 2);
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


	Object backfaceCulling(Object o) {
		Position dist = objectCenter(o);
		Vector3d camera, object;
		int j, s;
		s = o.shape.faceAmount;
		camera.x = this->frontDirection.x; camera.y = this->frontDirection.y; camera.z = this->frontDirection.z;
		object.x = dist.x; object.y = dist.y; object.z = dist.z;

		for (j = 0; j < s; j++) {
			Face f = o.shape.Faces[j];
			int v0, v1, v2;
			v0 = o.shape.Faces[j].Vertices[0]-1;
			v1 = o.shape.Faces[j].Vertices[1]-1;
			v2 = o.shape.Faces[j].Vertices[2]-1;
			Vertex a, b, c;
			a = o.shape.Vertices[v0];
			b = o.shape.Vertices[v1];
			c = o.shape.Vertices[v2];
			float area = -(((a.x*b.y)-(a.y*b.x)) + ((b.x*c.y)-(b.y*c.x)) + ((c.x*a.y)-(c.y*a.x)));
			if (area < 0) {
				o.shape.hideFace(j);
				printf("hide face\n");
			}
			/*Vector3d n;
			n.x = o.shape.MeshNormals[j].x;
			n.y = o.shape.MeshNormals[j].y;
			n.z = o.shape.MeshNormals[j].z;
			Vector3d cameraToObjectFace;
			float a = scalarProduct(n, camera);
			Vector3d r = normalizeVector(cameraToObjectFace);
			r = sumVectors(n, r);
			if ((a >= 0) && !(r.z == 0)) {
				o.shape.hideFace(j);
			}*/
		}
		return o;
	}
	bool visible(Object o, Camera* c) {
		//test if a object is invisible for the camera
		Position p = objectCenter(o);
		Vector3d v;
		v.x = p.x = c->pos.x;
		v.y = p.y = c->pos.y;
		v.z = p.z = c->pos.z;
		float angle = scalarProduct(v, c->frontDirection);
		printf("Angle %f\n", angle);
		if (angle > 0)//must be fixed
			return true;
		return false;
	}
	void updateVectorAngle() {
		//rotate the 3 vectors
		Angle a = this->angle;
		a.x = -this->angle.x; a.y = -this->angle.y; a.z = -this->angle.z;
		this->frontDirection = rotateVector(a, this->frontDirection);
		this->sideDirection = rotateVector(a, this->sideDirection);
		this->upDirection = rotateVector(a, this->upDirection);
	}

	void cameraRender(DisplayList l, SDL_Renderer* gRenderer) {
		//TODO deltas and blablabla then render
		int i;
		int s = l.objects.size();
		l = toCameraCoordinates(l);
		bool removed = false;
		//vector<bool> hiddenFaces;
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			o.sortZ(this->pos.z);
			o = setMeshNormals(o);
			o = prepareAndRotate(o);
			o = backfaceCulling(o);
			if (!visible(o, this)) {
				l.removeIndex(i);
				removed = true;
			}
			if (!removed)
				l.objects[i] = o.shape;
		}
		
		l = applyPerspective(l);

		//renderWireframe(gRenderer, l);
		render(gRenderer, l);
	}
};