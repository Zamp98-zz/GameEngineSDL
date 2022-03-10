#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <SDL_stdinc.h>
//take vector operations from camera
float correctAngle(float angle) {
	float bf = angle;
	angle = angle;
	printf("cuuuu %f\n", 2 * M_PI * RADIAN);

	if (angle > 0)
		while (angle > 2 * M_PI * RADIAN)
			angle = angle - 2 * M_PI * RADIAN;
	else if (angle < 0)
		while (angle < -2 * M_PI * RADIAN)
			angle = angle + 2 * M_PI * RADIAN;
	printf("angle before %f after %f\n", bf, angle);
	return angle;
}