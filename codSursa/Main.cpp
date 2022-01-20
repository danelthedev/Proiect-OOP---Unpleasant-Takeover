//Using SDL and standard IO
#include "Game.h"
#include <SDL.h>

#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

int main(int argc, char* args[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Game* game = new Game();
	game->init("Unpleasant Takeover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	//SDL_GL_SetSwapInterval(1);
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		
	}

	game->clean();
	delete game;

	std::cout << "Memory leaks: " << _CrtDumpMemoryLeaks() << '\n';

	return 0;
}