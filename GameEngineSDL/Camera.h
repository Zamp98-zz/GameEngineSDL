#pragma once
#include "SoftwareRenderer.h"
#include "Matrix.h"
#include "ObjLoader.h"
#include <SDL.h>



;

typedef struct angle {
	float x;
	float y;
	float z;
}Angle;


double min(double a, double b) {
	if (a == b || a < b) return a;
	else return b;
}

class Camera {
public:
	int fov;
	Matrix perspective;
	Position pos;
	Angle angle;
	Resolution res;
	Camera() {
		fov = 90.0;
		setProjectionMatrix(fov, 0.1, 100);
		pos.x = pos.y = pos.z = 0.0;
		angle.x = angle.z = angle.y = 0.0;
		
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
	}
	void rotateY(float s) {
		angle.y += s;
	}
	void rotateZ(float s) {
		angle.z += s;
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
	void cameraRender(DisplayList l, SDL_Renderer *gRenderer) {
		//TODO deltas and blablabla then render
		int i;
		int s = l.objects.size();
		l = toCameraCoordinates(l);
		for (i = 0; i < s; i++) {
			Object o = l.objects[i];
			o = prepareRotation(o);
			if (angle.x != 0)
				o = rotateObject(o, X, angle.x);
			if (angle.y != 0)
				o = rotateObject(o, Y, angle.y);
			if (angle.z != 0)
				o = rotateObject(o, Z, angle.z);
			Position dist = distance(o.pos, pos);//distance from camera to the object
			if (dist.z <= 1)//if its too close or behind the camera
				l.removeIndex(i);
			else
				l.objects[i] = o.shape;

		}
		l = applyPerspective(l);
		
		render(gRenderer, l);
	}
};
