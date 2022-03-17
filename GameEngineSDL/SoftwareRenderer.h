#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include "ObjLoader.h"
#include <SDL.h>
#include "Matrix.h"
#include "Position.h"


#define X -1
#define Y 0
#define Z 1
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

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
		bool compare(Position a, Position b, float z) {
			//printf("expression %f %f\n", z - a.z, z - b.z);
			return ( abs(z - a.z) < abs(z - b.z) );
		}
		Position faceCenter(Object *o, int i) {
			int v0, v1, v2;
			Vertex* v = o->getVerticesFromFace(i);
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
		void sortZ(float z) {//bubble sort
			int s = this->shape.faceAmount;
			int i;
			Position a, b;
			printf("sort in z = %f\n", z);
			for (i = 0; i < s; i++) {
				a = faceCenter(this, i);
				//printf("distance from camera in z: %f before\n", abs(z - a.z));
			}
			for (i = 0; i < s-1; i++) {
				int j;
				for (j = 0; j < s-1; j++) {
					b = faceCenter(this, j);
					a = faceCenter(this, i);
					if (compare(a, b, z)) {
						Face temp = this->shape.Faces[i];
						this->shape.Faces[i] = this->shape.Faces[j];
						this->shape.Faces[j] = temp;
						//printf("swap face %d %d %d with face %d %d %d\n", this->shape.Faces[j].Vertices[0], this->shape.Faces[i].Vertices[1], this->shape.Faces[i].Vertices[2],
							//this->shape.Faces[j].Vertices[0], this->shape.Faces[j].Vertices[1], this->shape.Faces[j].Vertices[2]);
					}
				}
			}
			for (i = 0; i < s; i++) {
				a = faceCenter(this, i);
				printf("distance from camera in z: %f after\n", abs(z - a.z));
			}
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