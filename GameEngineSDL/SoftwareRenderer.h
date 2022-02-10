#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ObjLoader.h"
#include <SDL.h>

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

class Perspective: public Position {
	public:
		float matrix[4][4];
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

void renderWireframe(SDL_Renderer* gRenderer, DisplayList l);
DisplayList applyPerspective(DisplayList l, Perspective p);