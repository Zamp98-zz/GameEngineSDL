/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "ObjLoader.h"
#include "SoftwareRenderer.h"
#include "Camera.h"


//Screen dimension constants


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	int px = 0;
	int py = 0;
	float s = 100.0;
	
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			Entity cube = loadEntity("Entity/cube.obj");
			Entity sphere = loadEntity("Entity/sphere.obj");
			//Entity map = loadEntity("Entity/MAPHOUSE.obj");
			//Entity c2 = loadEntity("Entity/c2.obj");
			DisplayList list;
			//list.insert(sphere);
			list.insert(cube);
			//list.insert(map);
			list = scale(s, list);
			Camera c;
			c.fov = 160;
			c.pos.y = 0;
			c.pos.x = 0;
			c.pos.z = 400.0;
			c.direction.z = 1;
			c.setResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0) {
					//User requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					switch (e.key.keysym.sym) {
					case SDLK_UP:
						//list = rotateObjects(list, .33, X);
						c.rotateX(0.01);
						break;
					case SDLK_DOWN:
						//list = rotateObjects(list, -.33, X);
						c.rotateX(-0.01);
						break;
					case SDLK_LEFT:
						//list = rotateObjects(list, .33, Y);
						c.rotateY(0.01);
						break;
					case SDLK_RIGHT:
						c.rotateY(-0.01);
						//list = rotateObjects(list, -0.33, Y);
						break;
					case SDLK_a:
						printf("a\n");
						//list = translate(list, X, -1000);
						c.translateX(100);
						break;
					case SDLK_s:
						printf("s\n");
						//list = translate(list, Y, 1000);
						c.translateY(-100);
						break;
					case SDLK_d:
						printf("d\n");
						//list = translate(list, X, 1000);
						c.translateX(-100);
						break;
					case SDLK_w:
						printf("w\n");
						//list = translate(list, Y, -1000);
						c.translateY(100);
						break;
					case SDLK_q:
						printf("q\n");
						//list = translate(list, Z, -1000);
						c.translateZ(-100);
						break;
					case SDLK_e:
						printf("e\n");
						//list = translate(list, Z, 1000);
						c.translateZ(100);
						break;
					case SDLK_x:
						printf("x\n");
						list = scale(1.3, list);
						break;
					case SDLK_z:
						printf("z\n");
						list = scale(0.9, list);
						break;
					default:
						break;
					}
					
					printf("Camera parameters: angle (x, y, z): %f %f %f\n pos (x, y, z): %f %f %f direction vec X:%f Y:%f Z:%f\n",
						c.angle.x*100, c.angle.y*100, c.angle.z*100,
						c.pos.x, c.pos.y, c.pos.z,
						c.direction.x, c.direction.y, c.direction.z);
					//Clear screen
					//Update screen
					DisplayList temp = list;
					//temp = applyDelta(c, temp);
					c.cameraRender(temp, gRenderer);
					//render(gRenderer, temp);
					//renderWireframe(gRenderer, temp);
					SDL_RenderPresent(gRenderer);
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
					SDL_RenderClear(gRenderer);
					SDL_Delay(33);
					
				}
			}
		}
		//Free resources and close SDL
		close();
		return 0;
	}
}