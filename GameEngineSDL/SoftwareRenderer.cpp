#include "SoftwareRenderer.h"
#include <math.h>


float M[4][4];

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
}

DisplayList scale(float s, DisplayList l) {
	int t = l.objects.size();
	int i;
	for (i = 0; i < t; i++) {
		int t = l.objects[i].Vertices.size();
		int j;
		for (j = 0; j < t; j++) {
			Matrix temp;
			//temp.init();
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
	return l;
}

SDL_Renderer* renderWireframe(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	int t = l.objects.size();
	int i;
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

			x0 = l.objects[i].Vertices[v0].x;
			y0 = l.objects[i].Vertices[v0].y;

			x1 = l.objects[i].Vertices[v1].x;
			y1 = l.objects[i].Vertices[v1].y;

			x2 = l.objects[i].Vertices[v2].x;
			y2 = l.objects[i].Vertices[v2].y;
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawLine(gRenderer, x0, y0, x1, y1);//v0 to v1
			SDL_RenderDrawLine(gRenderer, x0, y0, x2, y2);//v0 to v1
			SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);//v1 to v2

		}
		return gRenderer;
	}
}

DisplayList applyPerspective(DisplayList l, Perspective p) {
	int i;
	for (i = 0; i < l.objects.size(); i++) {
		int j;
		for (j = 0; j < l.objects[i].Vertices.size(); j++) {
			Position original;
			original.x = l.objects[i].Vertices[j].x;
			original.y = l.objects[i].Vertices[j].y;
			original.z = l.objects[i].Vertices[j].z;
			l.objects[i].Vertices[j].x = original.x * p.z / (p.z + original.z);
			l.objects[i].Vertices[j].y = original.y * p.z / (p.z + original.z);
		}
	}
	return l;
}