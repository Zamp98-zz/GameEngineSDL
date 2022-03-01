#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "ObjLoader.h"
#include <SDL.h>
#include "Matrix.h"
#include "Position.h"
#define X -1
#define Y 0
#define Z 1
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

using namespace std;


class Resolution {
public:
	int width;
	int height;
	Resolution() {
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}
};

class Object {
	public:
		Position pos;
		//Entity body;
		float mass;
		//Speed speed;
		float sc;
		Entity shape;//geometric informations
		Object(Entity e) {
			shape = e;
			mass = 0;
		}
		Vertex *getVerticesFromFace(int i) {
			Vertex r[3];
			r[0] = shape.Vertices[shape.Faces[i].Vertices[0] - 1];
			r[1] = shape.Vertices[shape.Faces[i].Vertices[1] - 1];
			r[2] = shape.Vertices[shape.Faces[i].Vertices[2] - 1];
			return r;
		}
		
};

class DisplayList {
	public:
		vector<Entity> objects;

		void insert(Entity object) {
			objects.push_back(object);
		}
		void removeIndex(int i) {
			objects.erase(objects.begin() + i);
		}
		void removeLast(Entity object) {
			objects.pop_back();
		}
	
};

DisplayList scale(float s, DisplayList l);
void renderWireframe(SDL_Renderer* gRenderer, DisplayList l);
void render(SDL_Renderer* gRenderer, DisplayList l);
void renderObject(Object o, SDL_Renderer* gRenderer);
DisplayList rotateObjects(DisplayList l, float angle, int axis);  
Object translate(Object o, int axis, float delta);
Object centerScreen(Object o, Resolution r);
Position objectCenter(Object o);
Object rotateObject(Object o, int axis, float angle);