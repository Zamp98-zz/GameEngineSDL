/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "ObjLoader.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//Key press surfaces constants



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}



bool loadMedia(std::string path)
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP(path.c_str());
	if (gHelloWorld == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		success = false;
	}
	printf("image loaded!\n");
	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[]){
	//Initialize SDL
	bool quit = false;
	//SDL_RendererInfo info;
	//SDL_Renderer *renderer = SDL_CreateRenderer(gWindow, 0, 0);

	//SDL_GetRenderDriverInfo(renderer, info);
	//printf("%s", info.name);
	std::string media = "hello.bmp";
	SDL_Event e;
	if (!init()){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		quit = true;
	}
	if (!loadMedia("hello.bmp")) {
		printf("Failed to load media! %s\n", SDL_GetError());
		quit = true;
	}
	Entity a = loadEntity("Entity/cube.obj");
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			switch (e.key.keysym.sym) {
				case SDLK_UP:
					printf("up arrow pressed!\n");
					loadMedia("upArrow.bmp");
					break;
				case SDLK_DOWN:
					printf("down arrow pressed!\n");
					loadMedia("downArrow.bmp");
					break;
				case SDLK_LEFT:
					printf("left arrow pressed!\n");
					loadMedia("leftArrow.bmp");
					break;
				case SDLK_RIGHT:
					printf("right arrow pressed!\n");
					loadMedia("rightArrow.bmp");
					break;
				default:
					printf("%s", e.key.keysym);
					loadMedia("hello.bmp");
					break;
			}
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
			SDL_Delay(100);
		}
	}
	//Destroy window
	SDL_DestroyWindow(gWindow);
	//Quit SDL subsystems
	SDL_Quit();
	return 0;
}