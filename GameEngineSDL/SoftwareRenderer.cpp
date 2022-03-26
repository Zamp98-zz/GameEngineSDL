#include "SoftwareRenderer.h"
#include <math.h>

float originalScale = 1;



DisplayList scale(float s, DisplayList l) {
	int t = l.objects.size();
	int i;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[0][1] = l.objects[i].Vertices[j].y;
			temp.values[0][2] = l.objects[i].Vertices[j].z;
			temp.values[0][3] = l.objects[i].Vertices[j].w;
			temp = temp.scaleMatrix(temp.values, s);
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = temp.values[0][3];
		}
	}
	originalScale = s;
	return l;
}

Object rotateObject(Object o, int axis, float angle) {
	int i;
	int v = o.shape.vertexAmount;
	for (i = 0; i < v; i++) {
		Matrix temp;

		temp.values[0][0] = o.shape.Vertices[i].x;
		temp.values[0][1] = o.shape.Vertices[i].y;
		temp.values[0][2] = o.shape.Vertices[i].z;
		temp.values[0][3] = o.shape.Vertices[i].w;
		if (axis == X) {
			temp = temp.rotateX(temp.values, angle);
		}
		else if (axis == Y) {
			temp = temp.rotateY(temp.values, angle);
		}
		else {
			temp = temp.rotateZ(temp.values, angle);
		}
		o.shape.Vertices[i].x = temp.values[0][0];
		o.shape.Vertices[i].y = temp.values[0][1];
		o.shape.Vertices[i].z = temp.values[0][2];
		o.shape.Vertices[i].w = temp.values[0][3];
	}
	return o;
}

DisplayList rotateObjects(DisplayList l, float angle, int axis) {
	int t = l.objects.size();
	int i;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[0][1] = l.objects[i].Vertices[j].y;
			temp.values[0][2] = l.objects[i].Vertices[j].z;
			temp.values[0][3] = l.objects[i].Vertices[j].w;
			if (axis == X) {
				temp = temp.rotateX(temp.values, angle);
			}
			else if (axis == Y) {
				temp = temp.rotateY(temp.values, angle);
			}
			else {
				temp = temp.rotateZ(temp.values, angle);
			}
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = temp.values[0][3];
		}
	}
	return l;
}

bool outOfBounds(SDL_Vertex vert) {
	bool x, y;
	x = y = false;
	if ((vert.position.x >= 0 && vert.position.x <= SCREEN_WIDTH) && (vert.position.y >= 0 && vert.position.y <= SCREEN_HEIGHT))
		return true;
	return false;
}

void renderObject(Object o, SDL_Renderer* gRenderer) {
	int i;
	int m = o.shape.faceAmount;
	for (i = 0; i < m; i++) {
		int v0, v1, v2;//vertex 1, 2 and 3 of the triangle
		if (o.shape.Faces[i].visible) {
			
			v0 = o.shape.Faces[i].Vertices[0] - 1;
			v1 = o.shape.Faces[i].Vertices[1] - 1;
			v2 = o.shape.Faces[i].Vertices[2] - 1;
			
			
			SDL_Vertex vert[3];
			vert->color.r = o.shape.Faces[i].color.red;
			vert->color.g = o.shape.Faces[i].color.green;
			vert->color.b = o.shape.Faces[i].color.blue;

			vert[0].position.x = o.shape.Vertices[v0].x;
			vert[0].position.y = o.shape.Vertices[v0].y;
			
			vert[0].color.r = 255;
			vert[0].color.g = 0;
			vert[0].color.b = 0;
			vert[0].color.a = 255;

			vert[1].position.x = o.shape.Vertices[v1].x;
			vert[1].position.y = o.shape.Vertices[v1].y;
			vert[1].color.r = 0;
			vert[1].color.g = 0;
			vert[1].color.b = 255;
			vert[1].color.a = 255;

			vert[2].position.x = o.shape.Vertices[v2].x;
			vert[2].position.y = o.shape.Vertices[v2].y;
			vert[2].color.r = 0;
			vert[2].color.g = 255;
			vert[2].color.b = 0;
			vert[2].color.a = 255;

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			if (outOfBounds(vert[0]) || outOfBounds(vert[1]) || outOfBounds(vert[2]))
				SDL_RenderGeometry(gRenderer, NULL, vert, 3, NULL, 0);
		}
		
	}
}
void render(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
	
	
	//l = scale(originalScale, l);
	for (i = 0; i < t; i++) {
		Object o = l.objects[i];
		renderObject(o, gRenderer);
		//return gRenderer;
	}
}

void renderObjectWireframe(Object o, SDL_Renderer *gRenderer){
	int i;
	int m = o.shape.faceAmount;
	for (i = 0; i < m; i++) {//the amount of meshes to be rendered
		int x;
		int y;
		int v0, v1, v2;//vertex 1, 2 and 3 of the triangle
		v0 = o.shape.Faces[i].Vertices[0] - 1;
		v1 = o.shape.Faces[i].Vertices[1] - 1;
		v2 = o.shape.Faces[i].Vertices[2] - 1;
		float x0, x1, x2, y0, y1, y2;

		SDL_Vertex vert[3];

		x0 = o.shape.Vertices[v0].x;
		y0 = o.shape.Vertices[v0].y;

		vert[0].position.x = x0;
		vert[0].position.y = y0;
		vert[0].color.r = 255;
		vert[0].color.g = 0;
		vert[0].color.b = 0;
		vert[0].color.a = 255;

		x1 = o.shape.Vertices[v1].x;
		y1 = o.shape.Vertices[v1].y;


		vert[1].position.x = x1;
		vert[1].position.y = y1;
		vert[1].color.r = 0;
		vert[1].color.g = 0;
		vert[1].color.b = 255;
		vert[1].color.a = 255;

		x2 = o.shape.Vertices[v2].x;
		y2 = o.shape.Vertices[v2].y;
		vert[2].position.x = x2;
		vert[2].position.y = y2;
		vert[2].color.r = 0;
		vert[2].color.g = 255;
		vert[2].color.b = 0;
		vert[2].color.a = 255;

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(gRenderer, x0, y0, x1, y1);//v0 to v1
		SDL_RenderDrawLine(gRenderer, x0, y0, x2, y2);//v0 to v1
		SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);//v1 to v2
	}
}

void renderWireframe(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
	
	for (i = 0; i < t; i++) {
		int u = l.objects[i].faceAmount;
		Object o = l.objects[i];
		renderObjectWireframe(o, gRenderer);
	}
}

Position objectCenter(Object o) {
	int t = o.shape.vertexAmount;
	int i;
	Position v;
	v.x = v.y = v.z = 0;
	for (i = 0; i < t; i++) {
		v.x += o.shape.Vertices[i].x;
		v.y += o.shape.Vertices[i].y;
		v.z += o.shape.Vertices[i].z;
	}
	v.x = v.x / t;
	v.y = v.y / t;
	v.z = v.z / t;
	return v;
}

Object centerScreen(Object o, Resolution r) {
	float tX = r.width / 2;
	float tY = r.height / 2;
	//translate
	
	//should center only one object!

	Position p = objectCenter(o);
	Position delta;
	delta.x = p.x - tX;
	o.pos = p;
	delta.y = p.y - tY;
	o = translate(o, X, -delta.x);
	o = translate(o, Y, -delta.y);
	return o;
}

Vertex translateVertex(Vertex v, int axis, float delta) {
	return v;
}

Object translate(Object o, int axis, float delta) {
	int t = o.shape.vertexAmount;
	int i;
	//delta = delta / 100;
	
	Matrix p = o.pos.toMatrixColumn(o.pos);

	if (axis == X) {
		p = p.translateX(p.values, delta);
	}
	if (axis == Y) {
		p = p.translateY(p.values, delta);
	}
	if (axis == Z) {
		p = p.translateZ(p.values, delta);
	}
	o.pos = o.pos.columnToPosition(p);
	for (i = 0; i < t; i++) {
		

		
		Matrix temp;
		
		//column vector
		temp.values[0][0] = o.shape.Vertices[i].x;
		temp.values[1][0] = o.shape.Vertices[i].y;
		temp.values[2][0] = o.shape.Vertices[i].z;
		temp.values[3][0] = o.shape.Vertices[i].w;
		if (axis == X) {
			temp = temp.translateX(temp.values, delta);
		}
		if (axis == Y) {
			temp = temp.translateY(temp.values, delta);
		}
		if(axis == Z){
			temp = temp.translateZ(temp.values, delta);
		}
		o.shape.Vertices[i].x = temp.values[0][0];
		o.shape.Vertices[i].y = temp.values[1][0];
		o.shape.Vertices[i].z = temp.values[2][0];
		o.shape.Vertices[i].w = temp.values[3][0];
	}
	return o;
}
