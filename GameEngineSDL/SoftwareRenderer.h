#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ObjLoader.h"
#include <SDL.h>
#include "Matrix.h"
#define X -1
#define Y 0
#define Z 1


using namespace std;

typedef struct position {
	float x;
	float y;
	float z;
}Position;
typedef struct speed {
	float x;
	float y;
	float z;
}Speed;


class Object {
	public:
		Position pos;
		Entity body;
		float mass;
		Speed speed;
		void rotate(float degX, float degY, float degZ) {
		}
		void translate(float x, float y, float z) {

		}
		void scale(float s) {
			
		}
};

class Resolution {
	public:
		int width;
		int height;
		Resolution(int w, int h) {
			width = w;
			height = h;
		}
};

class Perspective: public Position {
	public:
		float matrix[DIMENSION][DIMENSION];
		bool isSet;
		Perspective() {
			int i;
			int j;
			for (i = 0; i < DIMENSION; i++)
				for (j = 0; j < DIMENSION; j++)
					matrix[i][j] = 0;
			isSet = false;
		}

		void setProjectionMatrix(const float& angleOfView, const float& near, const float& far)
		{
			// set the basic projection matrix
			float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
			matrix[0][0] = scale; // scale the x coordinates of the projected point 
			matrix[1][1] = scale; // scale the y coordinates of the projected point 
			matrix[2][2] = -far / (far - near); // used to remap z to [0,1] 
			matrix[3][2] = -far * near / (far - near); // used to remap z [0,1] 
			matrix[2][3] = -1; // set w = -z 
			matrix[3][3] = 0;
			isSet = true;
		}
};

class Camera: public Object{
	public:
		int fov;
		Perspective perspective;
};

class DisplayList {
	public:
		vector<Entity> objects;

		void insert(Entity object) {
			objects.push_back(object);
		}
		void removeLast(Entity object) {
			objects.pop_back();
		}
		void sort(Camera c) {
		}
	
};


DisplayList scale(float s, DisplayList l);
SDL_Renderer * renderWireframe(SDL_Renderer* gRenderer, DisplayList l);
DisplayList applyPerspective(DisplayList l, Perspective p);
DisplayList rotateObjects(DisplayList l, float angle, int axis);  
DisplayList translate(DisplayList l, int axis, float delta);