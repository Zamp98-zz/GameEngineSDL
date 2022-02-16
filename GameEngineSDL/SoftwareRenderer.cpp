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


void render(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
	Perspective p;
	if (!p.isSet)
		p.setProjectionMatrix(90, 0.1, 100);
	l = applyPerspective(l, p);
	l = scale(originalScale, l);
	for (i = 0; i < t; i++) {
		int u = l.objects[i].faceAmount;
		int j;
		for (j = 0; j < u; j++) {//run the face amount
			int v0, v1, v2;//vertex 1, 2 and 3 of the triangle
			v0 = l.objects[i].Faces[j].Vertices[0] - 1;
			v1 = l.objects[i].Faces[j].Vertices[1] - 1;
			v2 = l.objects[i].Faces[j].Vertices[2] - 1;

			SDL_Vertex vert[3];

			vert[0].position.x = l.objects[i].Vertices[v0].x;
			vert[0].position.y = l.objects[i].Vertices[v0].y;
			vert[0].color.r = 255;
			vert[0].color.g = 0;
			vert[0].color.b = 0;
			vert[0].color.a = 255;


			vert[1].position.x = l.objects[i].Vertices[v1].x;
			vert[1].position.y = l.objects[i].Vertices[v1].y;
			vert[1].color.r = 0;
			vert[1].color.g = 0;
			vert[1].color.b = 255;
			vert[1].color.a = 255;

			vert[2].position.x = l.objects[i].Vertices[v2].x;
			vert[2].position.y = l.objects[i].Vertices[v2].y;
			vert[2].color.r = 0;
			vert[2].color.g = 255;
			vert[2].color.b = 0;
			vert[2].color.a = 255;
			
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderGeometry(gRenderer, NULL, vert, 3, NULL, 0);

		}
		//return gRenderer;
	}
}

void renderWireframe(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
	Perspective p;
	if(!p.isSet)
		p.setProjectionMatrix(90, 0.1, 100);
	l = applyPerspective(l, p);
	for (i = 0; i < t; i++) {
		int u = l.objects[i].faceAmount;
		int j;
		for (j = 0; j < u; j++) {//run the face amount
			int x;
			int y;
			int v0, v1, v2;//vertex 1, 2 and 3 of the triangle
			v0 = l.objects[i].Faces[j].Vertices[0] - 1;
			v1 = l.objects[i].Faces[j].Vertices[1] - 1;
			v2 = l.objects[i].Faces[j].Vertices[2] - 1;
			float x0, x1, x2, y0, y1, y2;

			SDL_Vertex vert[3];

			x0 = l.objects[i].Vertices[v0].x;
			y0 = l.objects[i].Vertices[v0].y;

			vert[0].position.x = x0;
			vert[0].position.y = y0;
			vert[0].color.r = 255;
			vert[0].color.g = 0;
			vert[0].color.b = 0;
			vert[0].color.a = 255;

			x1 = l.objects[i].Vertices[v1].x;
			y1 = l.objects[i].Vertices[v1].y;
			
			
			vert[1].position.x = x1;
			vert[1].position.y = y1;
			vert[1].color.r = 0;
			vert[1].color.g = 0;
			vert[1].color.b = 255;
			vert[1].color.a = 255;

			x2 = l.objects[i].Vertices[v2].x;
			y2 = l.objects[i].Vertices[v2].y;
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
			//SDL_RenderGeometry(gRenderer, NULL, vert, 3, NULL, 0);

		}
		//return gRenderer;
	}
}

DisplayList center(DisplayList l, Resolution r) {
	float tX = r.width / 2;
	float tY = r.height / 2;
	//translate
	return l;
}

DisplayList applyPerspective(DisplayList l, Perspective p) {
	int i;
	for (i = 0; i < l.objects.size(); i++) {
		int j;
		for (j = 0; j < l.objects[i].Vertices.size(); j++) {
			/*Position original;
			original.x = l.objects[i].Vertices[j].x;
			original.y = l.objects[i].Vertices[j].y;
			original.z = l.objects[i].Vertices[j].z;
			l.objects[i].Vertices[j].x = original.x * p.z / (p.z + original.z);
			l.objects[i].Vertices[j].y = original.y * p.z / (p.z + original.z);*/
			//out = in * M;
			Matrix temp;
			temp.init();
			temp.values[0][0] = l.objects[i].Vertices[j].x * p.matrix[0][0] + l.objects[i].Vertices[j].y * p.matrix[1][0] + l.objects[i].Vertices[j].z * p.matrix[2][0] + /* in.z = 1 */ p.matrix[3][0];
			temp.values[0][1] = l.objects[i].Vertices[j].x * p.matrix[0][1] + l.objects[i].Vertices[j].y * p.matrix[1][1] + l.objects[i].Vertices[j].z * p.matrix[2][1] + /* in.z = 1 */ p.matrix[3][1];
			temp.values[0][2] = l.objects[i].Vertices[j].x * p.matrix[0][2] + l.objects[i].Vertices[j].y * p.matrix[1][2] + l.objects[i].Vertices[j].z * p.matrix[2][2] + /* in.z = 1 */ p.matrix[3][2];
			float w = l.objects[i].Vertices[j].x * p.matrix[0][3] + l.objects[i].Vertices[j].y * p.matrix[1][3] + l.objects[i].Vertices[j].z * p.matrix[2][3] + /* in.z = 1 */ p.matrix[3][3];

			// normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
			if (w != 1) {
				temp.values[0][0] /= w;
				temp.values[0][1] /= w;
				temp.values[0][2] /= w;
			}
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[0][1];
			l.objects[i].Vertices[j].z = temp.values[0][2];
			l.objects[i].Vertices[j].w = w;
		}
	}
	return l;
}
DisplayList translate(DisplayList l, int axis, float delta) {
	int t = l.objects.size();
	int i;
	delta = delta / 100;
	for (i = 0; i < t; i++) {
		int u = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < u; j++) {
			Matrix temp;
			temp.init();
			//column vector
			temp.values[0][0] = l.objects[i].Vertices[j].x;
			temp.values[1][0] = l.objects[i].Vertices[j].y;
			temp.values[2][0] = l.objects[i].Vertices[j].z;
			temp.values[3][0] = l.objects[i].Vertices[j].w;
			if (axis == X) {
				temp = translateX(temp.values, delta);
			}
			else if (axis == Y) {
				temp = translateY(temp.values, delta);
			}
			else if(axis == Z){
				temp = translateZ(temp.values, delta);
			}
			l.objects[i].Vertices[j].x = temp.values[0][0];
			l.objects[i].Vertices[j].y = temp.values[1][0];
			l.objects[i].Vertices[j].z = temp.values[2][0];
			l.objects[i].Vertices[j].w = temp.values[3][0];
		}
	}
	return l;
}