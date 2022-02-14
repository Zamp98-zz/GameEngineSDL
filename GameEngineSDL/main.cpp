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

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	float x = 100.0;
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
			//Entity cube = loadEntity("Entity/cube.obj");
			Entity map = loadEntity("Entity/MAPHOUSE.obj");
			DisplayList list;
			//list.insert(cube);
			list.insert(map);
			list = scale(x, list);
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0) {
					//User requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					switch (e.key.keysym.sym) {
					case SDLK_UP:
						//printf("up arrow pressed!\n");
						//loadMedia("upArrow.bmp");
						list = rotateObjects(list, .33, X);
						//py--;
						break;
					case SDLK_DOWN:
						//printf("down arrow pressed!\n");
						//loadMedia("downArrow.bmp");
						list = rotateObjects(list, -.33, X);
						//py++;
						break;
					case SDLK_LEFT:
						//printf("left arrow pressed!\n");
						//loadMedia("leftArrow.bmp");
						list = rotateObjects(list, .33, Y);
						//px--;
						break;
					case SDLK_RIGHT:
						//printf("right arrow pressed!\n");
						//loadMedia("rightArrow.bmp");
						list = rotateObjects(list, -0.33, Y);
						//px++;
						break;
					case SDLK_a:
						list = translate(list, X, 10);
					case SDLK_s:
						list = translate(list, Y, -10);
					case SDLK_d:
						list = translate(list, X, -10);
					case SDLK_w:
						list = translate(list, Y, 10);
					/*case SDLK_x:
						printf("a");
						//list = scale(1.3, list);
					case SDLK_z:
						printf("z");
						//list = scale(0.9, list);*/
					default:
						//printf("%s", e.key.keysym);
						//loadMedia("hello.bmp");
						//float gx = 1;
						break;
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render red filled quad
					/*SDL_Rect fillRect;// = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
					fillRect.h = 20;
					fillRect.w = 20;
					fillRect.x = px;
					fillRect.y = py;
					
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
					SDL_RenderFillRect(gRenderer, &fillRect);
					*/
					//Render green outlined quad
					
					/*SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
					SDL_RenderDrawRect(gRenderer, &outlineRect);

					//Draw blue horizontal line
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
					SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

					//Draw vertical line of yellow dots
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
					for (int i = 0; i < SCREEN_HEIGHT; i += 4)
					{
						SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
					}
					*/
					//Update screen
					renderWireframe(gRenderer, list);
					SDL_RenderPresent(gRenderer);
					SDL_Delay(33);
				}
			}
		}

		//Free resources and close SDL
		close();

		return 0;
	}
}