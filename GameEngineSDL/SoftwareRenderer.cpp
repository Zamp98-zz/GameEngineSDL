#include "SoftwareRenderer.h"
#include <math.h>

Perspective p;

float M[4][4];
/*
void setProjectionMatrix(const float& angleOfView, const float& near, const float& far)
{
	// set the basic projection matrix
	float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
	M[0][0] = scale; // scale the x coordinates of the projected point 
	M[1][1] = scale; // scale the y coordinates of the projected point 
	M[2][2] = -far / (far - near); // used to remap z to [0,1] 
	M[3][2] = -far * near / (far - near); // used to remap z [0,1] 
	M[2][3] = -1; // set w = -z 
	M[3][3] = 0;
}*/
Resolution r;
float originalScale = 1;

DisplayList scale(float s, DisplayList l) {
	int t = l.objects.size();
	int i;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			temp.init();
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[0][1] = l.objects[i].Vertices[j].y;
			temp.values[0][2] = l.objects[i].Vertices[j].z;
			temp.values[0][3] = l.objects[i].Vertices[j].w;
			temp = scaleMatrix(temp.values, s);
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = temp.values[0][3];
		}
	}
	originalScale = s;
	return l;
}

DisplayList rotateObjects(DisplayList l, float angle, int axis) {
	int t = l.objects.size();
	int i;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			temp.init();
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[0][1] = l.objects[i].Vertices[j].y;
			temp.values[0][2] = l.objects[i].Vertices[j].z;
			temp.values[0][3] = l.objects[i].Vertices[j].w;
			if (axis == X) {
				temp = rotateX(temp.values, angle);
			}
			else if (axis == Y) {
				temp = rotateY(temp.values, angle);
			}
			else {
				temp = rotateZ(temp.values, angle);
			}
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = temp.values[0][3];
		}
	}
	return l;
}

bool outOfBounds() {
	return false;
}

void renderObject(Object o, SDL_Renderer* gRenderer) {
	int i;
	int m = o.shape.faceAmount;
	for (i = 0; i < m; i++) {
		int v0, v1, v2;//vertex 1, 2 and 3 of the triangle
		v0 = o.shape.Faces[i].Vertices[0] - 1;
		v1 = o.shape.Faces[i].Vertices[1] - 1;
		v2 = o.shape.Faces[i].Vertices[2] - 1;

		SDL_Vertex vert[3];
		
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
		SDL_RenderGeometry(gRenderer, NULL, vert, 3, NULL, 0);
	}
}
void render(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
	Perspective p;
	
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
	Perspective p;
	
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

DisplayList applyPerspective(DisplayList l, Perspective p) {
	int i;
	p.setProjectionMatrix(90, 0.1, 1000);
	for (i = 0; i < l.objects.size(); i++) {
		int j;
		for (j = 0; j < l.objects[i].Vertices.size(); j++) {

			Position original;
			original.x = l.objects[i].Vertices[j].x;
			original.y = l.objects[i].Vertices[j].y;
			original.z = l.objects[i].Vertices[j].z;
			l.objects[i].Vertices[j].x = original.x * p.Z0 / (p.Z0 + original.z);
			l.objects[i].Vertices[j].y = original.y * p.Z0 / (p.Z0 + original.z);
			l.objects[i].Vertices[j].z = original.z;
		}
	}
	return l;
}

Object translate(Object o, int axis, float delta) {
	int t = o.shape.vertexAmount;
	int i;
	//delta = delta / 100;
	
	Matrix p = o.pos.toMatrixColumn(o.pos);

	if (axis == X) {
		p = translateX(p.values, delta);
	}
	if (axis == Y) {
		p = translateY(p.values, delta);
	}
	if (axis == Z) {
		p = translateZ(p.values, delta);
	}
	o.pos = o.pos.columnToPosition(p);
	for (i = 0; i < t; i++) {
		

		
		Matrix temp;
		temp.init();
		//column vector
		temp.values[0][0] = o.shape.Vertices[i].x;
		temp.values[1][0] = o.shape.Vertices[i].y;
		temp.values[2][0] = o.shape.Vertices[i].z;
		temp.values[3][0] = o.shape.Vertices[i].w;
		if (axis == X) {
			temp = translateX(temp.values, delta);
		}
		if (axis == Y) {
			temp = translateY(temp.values, delta);
		}
		if(axis == Z){
			temp = translateZ(temp.values, delta);
		}
		o.shape.Vertices[i].x = temp.values[0][0];
		o.shape.Vertices[i].y = temp.values[1][0];
		o.shape.Vertices[i].z = temp.values[2][0];
		o.shape.Vertices[i].w = temp.values[3][0];
	}
	return o;
}
//apply position ant rotation delta for the camera

DisplayList applyDelta(Camera c, DisplayList l) {
	int t = l.objects.size();
	int i;

	Position delta;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		Object o = l.objects[i];
		Position center = objectCenter(o);

		o = centerScreen(o, r);
		l.objects[i] = o.shape;
	}
	return l;
}

/*DisplayList applyDelta(Camera c, DisplayList l) {
	int t = l.objects.size();
	int i;
	
	Position delta;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			temp.init();
			//column vector
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[0][1] = l.objects[i].Vertices[j].y;
			temp.values[0][2] = l.objects[i].Vertices[j].z;
			//temp.values[0][3] = l.objects[i].Vertices[j].w;
			//distance from camera to the vertex
			delta.x = (c.pos.x - l.objects[i].Vertices[j].x);
			delta.y = (c.pos.y - l.objects[i].Vertices[j].y);
			delta.z = (c.pos.z - l.objects[i].Vertices[j].z);

			//MOVE - (camera position)
			//temp.values[0][0] -= c.pos.x;
			//temp.values[0][1] -= c.pos.y;
			//temp.values[0][2] -= c.pos.z;
			if (c.angle.x != 0) {
				//move the vertex delta x, delta y and delta z to make the camera look the origin
				//rotate
				// move back to the original position
				temp = rotateX(temp.values, -c.angle.x);

				//l = rotateObjects(l, X, c.angle.x);
			}
			if (c.angle.y != 0) {
				//l = rotateObjects(l, Y, c.angle.y);
				temp = rotateY(temp.values, -c.angle.y);
			}
			if (c.angle.z != 0) {
				//l = rotateObjects(l, Z, c.angle.z);
				temp = rotateZ(temp.values, -c.angle.z);
			}
			//TODO restore the position
			//temp.values[0][0] += c.pos.x;
			//temp.values[0][1] += c.pos.y;
			//temp.values[0][2] += c.pos.z;
			//c.angle.x = c.angle.y = c.angle.z = 0;
			//translate according with the delta
			
			//l = translate(l, X, delta.x);
			if (delta.x == delta.y == 0) {//it's like the 
				l = centerScreen(l, r);
			}
			temp = translateX(temp.values, delta.x);
			
			
			//l = translate(l, Y, delta.y);
			temp = translateY(temp.values, delta.y);
			
			
			//l = translate(l, Z, delta.z);
			temp = translateZ(temp.values, delta.z);
			
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = temp.values[0][3];
			Resolution r;
			l = centerScreen(l, r);
		}
	}
	return l;
}*/