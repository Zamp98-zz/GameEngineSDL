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

SDL_Renderer * renderWireframe(SDL_Renderer* gRenderer, DisplayList l) {
	//TODO
	return gRenderer;
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