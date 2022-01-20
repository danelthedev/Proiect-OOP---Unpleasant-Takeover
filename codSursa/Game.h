#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Vector2D.h"

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();


	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() const { return isRunning; }
	void render();
	void clean();

	static bool playSound;
	static bool isPaused;

	static int gameState; //1 = playing
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static std::vector<Vector2D> enemycoords;

	static SDL_Rect camera;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupEnemies,
		groupBoolets,
		groupBooletsE,
		groupUI,
		groupButtons,
		groupConsumables
	};

private:
	int cnt = 0;
	SDL_Window* window;
};

