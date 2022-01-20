#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Game.h"
#include "Texture.h"
#include "Map.h"
#include "Components.h"
#include "Collision.h"
#include "SDL_ttf.h"
#include <windows.h>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 1366, 768 };

int hp = 100;

TTF_Font* font = NULL;

const Uint8* kbs = SDL_GetKeyboardState(NULL);

std::vector<Vector2D> Game::enemycoords;

bool Game::playSound = true;
bool Game::isPaused = false;

bool Game::isRunning = false;
int Game::gameState = -1;

int showTutorial = 0;
int currentLvl = 1;
int totalEnemies;

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& boolets(manager.getGroup(Game::groupBoolets));
auto& booletsE(manager.getGroup(Game::groupBooletsE));
auto& UIs(manager.getGroup(Game::groupUI));
auto& buttons(manager.getGroup(Game::groupButtons));
auto& consumables(manager.getGroup(Game::groupConsumables));


//obiecte


auto& player(manager.addEntity());
auto& gun(manager.addEntity());

auto& UIAmmo(manager.addEntity());
auto& UIHP(manager.addEntity());
 

auto& fundal(manager.addEntity());
auto& tutorial(manager.addEntity());
auto& levelComplete(manager.addEntity());
auto& gameComplete(manager.addEntity());
auto& playButton(manager.addEntity());
auto& tutorialButton(manager.addEntity());
auto& exitButton(manager.addEntity());
auto& danelButton(manager.addEntity());

//ingame menu button
bool isPaused = 0;
bool playSound = 1;

auto& IngameMenu(manager.addEntity());
auto& exitIngameButton(manager.addEntity());
auto& soundIngameButton(manager.addEntity());


SDL_Window* window = nullptr;
Uint32 fsState = 0;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		try
		{
			if (window == NULL)
				throw "Error creating window.";
			else
			{
				std::cout << "Window created\n";
			}
		}
		catch(const std::exception& e)
		{
			std::cout << e.what();
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		try
		{
			if (renderer == NULL)
				throw "Error creating renderer.";
			else
			{
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
				std::cout << "Renderer created\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	int initvar = TTF_Init();
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

	try
	{
		if (initvar == -1)
			throw "Error creating text.";
		else
		{
			std::cout << "Text loaded\n";
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	//button stuff

	fundal.addComponent<TransformComponent>(0, 0, 768, 1366, 1);
	fundal.addComponent<SpriteComponent>("assets/fundal.png", false);

	tutorial.addComponent<TransformComponent>(14, 292, 349, 1342, 1);
	tutorial.addComponent<SpriteComponent>("assets/tutorial.png", false);

	levelComplete.addComponent<TransformComponent>(216, 45, 112, 926, 1);
	levelComplete.addComponent<SpriteComponent>("assets/levelCompleted.png", false);

	gameComplete.addComponent<TransformComponent>(216, 300, 112, 926, 1);
	gameComplete.addComponent<SpriteComponent>("assets/Congrats!.png", false);

	playButton.addComponent<TransformComponent>(562, 260, 134, 256, 1);
	playButton.addComponent<SpriteComponent>("assets/PlayButton.png", false);
	playButton.addComponent<ButtonComponent>();
	playButton.addGroup(groupButtons);

	tutorialButton.addComponent<TransformComponent>(490, 437, 113, 400, 1);
	tutorialButton.addComponent<SpriteComponent>("assets/TutorialButton.png", false);
	tutorialButton.addComponent<ButtonComponent>();
	tutorialButton.addGroup(groupButtons);

	exitButton.addComponent<TransformComponent>(571, 590, 133, 238, 1);
	exitButton.addComponent<SpriteComponent>("assets/ExitButton.png", false);
	exitButton.addComponent<ButtonComponent>();
	exitButton.addGroup(groupButtons);

	danelButton.addComponent<TransformComponent>(50, 650, 100, 100, 1);
	danelButton.addComponent<SpriteComponent>("assets/profil64x64.png", false);
	danelButton.addComponent<ButtonComponent>();
	danelButton.addGroup(groupButtons);

}

void Game::handleEvents()
{
	while (SDL_PollEvent(&event))
	{
		if((bool)kbs[SDL_SCANCODE_ESCAPE])
		{
			if (Game::isPaused == 1)
				Game::isPaused = 0;
			else
				Game::isPaused = 1;
		}

		if (event.key.repeat == 1)
			continue;
	}

	switch (event.type)
	{
	case SDL_QUIT: {
		isRunning = false;
		break;

	}
	default:
		break;
	}
}

void Game::update()
{
	SDL_Rect playerCol;
	Vector2D playerPos;

	//UI STUFF
	if (Game::gameState == 1)
	{
		playerCol = player.getComponent<ColliderComponent>().collider;
		playerPos = player.getComponent<TransformComponent>().position;
		//update UI

		std::stringstream ss;

		ss << "Ammo: "
			+ std::to_string(player.getComponent<KeyboardController>().magCurAmmo[player.getComponent<KeyboardController>().usedWeapon])
			+ " / "
			+ std::to_string(player.getComponent<KeyboardController>().ammo[player.getComponent<KeyboardController>().usedWeapon]);

		UIAmmo.getComponent<UILabel>().SetLabelText(ss.str());

		ss.str("");

		ss << "Hp: " + std::to_string(hp);
		UIHP.getComponent<UILabel>().SetLabelText(ss.str());

		ss.clear();

	}

	//update all entitiess

	manager.refresh();
	manager.update();

	//ingame updates
	if (Game::gameState == 1 && isPaused == 0)
	{
		IngameMenu.getComponent<TransformComponent>().setPos(-1000, -1000);
		exitIngameButton.getComponent<TransformComponent>().setPos(-1000, -1000);
		soundIngameButton.getComponent<TransformComponent>().setPos(-1000, -1000);
		//game processes


		//change weapon appearance
		gun.getComponent<SpriteComponent>().setAnimSet(player.getComponent<KeyboardController>().usedWeapon);

		//collision checking
		for (auto& c : colliders) //coliziunea peretilor
		{
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider; //collider of wall

			for (auto& b : booletsE) //check collision of boolets with walls
			{
				SDL_Rect bCol = b->getComponent<ColliderComponent>().collider; //collider of boolet

				if (Collision::AABB(cCol, bCol))
				{
					b->destroy(); //destroy boolets in contact with walls
				}
			}

			for (auto& b : boolets) //check collision of boolets with walls
			{
				SDL_Rect bCol = b->getComponent<ColliderComponent>().collider; //collider of boolet

				if (Collision::AABB(cCol, bCol))
				{
					b->destroy(); //destroy boolets in contact with walls
				}
			}

			if (Collision::AABB(cCol, playerCol)) //coliziunea peretului cu playerul
			{
				player.getComponent<TransformComponent>().position = playerPos; // opreste playerul din a avansa
			}
		}

		if (player.getComponent<TransformComponent>().position.x <= 0)
			player.getComponent<TransformComponent>().position = playerPos;

		if (player.getComponent<TransformComponent>().position.y <= 0)
			player.getComponent<TransformComponent>().position = playerPos;

		if (player.getComponent<TransformComponent>().position.x >= 64 * 63)
			player.getComponent<TransformComponent>().position = playerPos;

		if (player.getComponent<TransformComponent>().position.y >= 64 * 63)
			player.getComponent<TransformComponent>().position = playerPos;

		for (auto& e : enemies) //parcurg toti inamicii
		{
			SDL_Rect eCol = e->getComponent<ColliderComponent>().collider; //collider inamici

			for (auto& b : boolets) //parcurg toate gloantele
			{
				SDL_Rect bCol = b->getComponent<ColliderComponent>().collider; //collider gloante

				if (Collision::AABB(eCol, bCol)) //daca se colizeaza, scade hp la inamic si distruge glontul
				{
					e->getComponent<EnemyInfo>().hp -= b->getComponent<ProjectileComponent>().dmg; //dai dmg
					if (e->getComponent<EnemyInfo>().hp <= 0) //daca hp < 0 distruge inamic
					{
						Mix_Chunk* bruh = NULL;
						bruh = Mix_LoadWAV("assets/bruh.wav");
						if (Game::playSound == true)
							Mix_PlayChannel(-1, bruh, 0);

						totalEnemies--;
						e->getComponent<TransformComponent>().position.x = -1000;
						e->getComponent<TransformComponent>().position.y = -1000;
						//(*e).destroy();
					}
					(*b).destroy();
				}
			}

		}

		for (auto& eb : booletsE)
		{
			SDL_Rect ebCol = eb->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(ebCol, player.getComponent<ColliderComponent>().collider))
			{
				hp -= 10;
				(*eb).destroy();
			}
		}


		//gun functionality
		gun.getComponent<TransformComponent>().copyTransform(&player.getComponent<TransformComponent>());

		if (player.getComponent<KeyboardController>().shot == 1)
		{
			//pozitia in care sa se spawneze glontu

			int px = player.getComponent<TransformComponent>().position.x + 32;
			int py = player.getComponent<TransformComponent>().position.y + 32;

			//gun animation

			gun.getComponent<SpriteComponent>().setFrame(1);

			//creating boolet

			if (player.getComponent<KeyboardController>().usedWeapon == 0) //pistol boolet
			{
				auto& boolet(manager.addEntity());

				boolet.addComponent<TransformComponent>(px, py, 0.25);
				boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
				boolet.addComponent<ProjectileComponent>(player.getComponent<TransformComponent>().rot, 25);
				boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
					boolet.getComponent<TransformComponent>().position.y, 16);
				boolet.addGroup(groupBoolets);
			}
			else
				if (player.getComponent<KeyboardController>().usedWeapon == 1) //shotgun boolets
				{
					int ang = player.getComponent<TransformComponent>().rot + rand() % 10 - 5;
					for (int i = 0; i < 8; ++i)
					{
						auto& boolet(manager.addEntity());
						boolet.addComponent<TransformComponent>(player.getComponent<TransformComponent>().position.x + 32, player.getComponent<TransformComponent>().position.y + 32, 0.25);
						boolet.getComponent<TransformComponent>().rot = (ang - 60) + i * 15;
						boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
						boolet.addComponent<ProjectileComponent>((ang - 60) + i * 15, 10);
						boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
							boolet.getComponent<TransformComponent>().position.y, 16);
						boolet.addGroup(Game::groupBoolets);
					}
				}
				else
					if (player.getComponent<KeyboardController>().usedWeapon == 2)
					{
						auto& boolet(manager.addEntity());

						boolet.addComponent<TransformComponent>(px, py, 0.25);
						boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
						boolet.addComponent<ProjectileComponent>(player.getComponent<TransformComponent>().rot, 25);
						boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
							boolet.getComponent<TransformComponent>().position.y, 16);
						boolet.addGroup(groupBoolets);

					}

		}
		else
			gun.getComponent<SpriteComponent>().setFrame(0);

		//camera clamp and movement

		camera.x = player.getComponent<TransformComponent>().position.x - 683;
		camera.y = player.getComponent<TransformComponent>().position.y - 384;

		if(currentLvl == 1)
		{
			if (camera.x < 0)
				camera.x = 0;
			if (camera.y < 0)
				camera.y = 0;
			if (camera.x > 32 * 64 - camera.w)
				camera.x = 32 * 64 - camera.w;
			if (camera.y > 32 * 64 - camera.h)
				camera.y = 32 * 64 - camera.h;
		}
		if (currentLvl == 2)
		{
			if (camera.x < 32 * 64)
				camera.x = 32 * 64; // stanga

			if (camera.y < 0)
				camera.y = 0; // sus

			if (camera.x > 64 * 64 - camera.w)
				camera.x = 64 * 64 - camera.w; //dreapta

			if (camera.y > 32 * 64 - camera.h)
				camera.y = 32 * 64 - camera.h; // jos
		}
		if (currentLvl == 3 || currentLvl == 4)
		{
			if (camera.x < 0)
				camera.x = 0; // stanga

			if (camera.y < 32 * 64)
				camera.y = 32 * 64; // sus

			if (camera.x > 64 * 64 - camera.w)
				camera.x = 64 * 64 - camera.w; //dreapta

			if (camera.y > 64 * 64 - camera.h)
				camera.y = 64 * 64 - camera.h; // jos
		}



		//lose state

		if (hp <= 0)
		{
			hp = 100;
			player.getComponent<KeyboardController>().ammo[0] = 35;
			player.getComponent<KeyboardController>().ammo[1] = 10;
			player.getComponent<KeyboardController>().ammo[2] = 60;


			if (currentLvl == 1)
			{
				player.getComponent<TransformComponent>().position.x = 1900;
				player.getComponent<TransformComponent>().position.y = 850;
				totalEnemies = 46;
			}
			if (currentLvl == 2) 
			{
				player.getComponent<TransformComponent>().position.x = 2264;
				player.getComponent<TransformComponent>().position.y = 850;
				totalEnemies = 37;
			}
			if (currentLvl == 3)
			{
				player.getComponent<TransformComponent>().position.x = 3080;
				player.getComponent<TransformComponent>().position.y = 2260;
				totalEnemies = 25;
			}

			for(int i = 0; i < enemycoords.size(); ++i)
			{
				enemies[i]->getComponent<TransformComponent>().position = enemycoords[i];
				enemies[i]->getComponent<EnemyInfo>().hp = 100;
			}

		}

		//level continues

		if (totalEnemies == 37 && currentLvl == 1 && (bool)kbs[SDL_SCANCODE_RETURN] == 1)
		{
			for (auto& t : tiles)
				t->destroy();
			for (auto& c : colliders)
				c->destroy();

			map->LoadMapChunk("assets/fullmap.txt", 32, 0, 65, 32);
			
	
			player.getComponent<TransformComponent>().position.x = 2264;
			player.getComponent<TransformComponent>().position.y = 850;
			currentLvl++;
		}
		if (totalEnemies == 25 && currentLvl == 2 && (bool)kbs[SDL_SCANCODE_RETURN] == 1)
		{
			for (auto& t : tiles)
				t->destroy();
			for (auto& c : colliders)
				c->destroy();

			map->LoadMapChunk("assets/fullmap.txt", 0, 32, 65, 65);

			player.getComponent<TransformComponent>().position.x = 3080;
			player.getComponent<TransformComponent>().position.y = 2260;
			currentLvl++;
		}

		if (totalEnemies == 0 && currentLvl == 3)
		{
			currentLvl = 4;

			Mix_Chunk* yay = NULL;
			yay = Mix_LoadWAV("assets/yay.wav");
			if (Game::playSound == true)
				Mix_PlayChannel(-1, yay, 0);
		}
		//consumable interaction

		for (auto& c : consumables)
		{
			//SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			SDL_Rect cCol;
			cCol.x = c->getComponent<TransformComponent>().position.x;
			cCol.y = c->getComponent<TransformComponent>().position.y;
			cCol.h = 64;
			cCol.w = 64;

			
			if (SDL_HasIntersection(&player.getComponent<ColliderComponent>().collider, &cCol))
			{
				if (c->getComponent<SpriteComponent>().getFrame() == 0 && (bool)kbs[SDL_SCANCODE_E] == true)
				{
					hp = 100;
					(*c).destroy();
				}

				if (c->getComponent<SpriteComponent>().getFrame() == 1 && (bool)kbs[SDL_SCANCODE_E] == true)
				{
					player.getComponent<KeyboardController>().ammo[0] = 35;
					player.getComponent<KeyboardController>().ammo[1] = 10;
					player.getComponent<KeyboardController>().ammo[2] = 60;
					(*c).destroy();
				}

			}
		}


	}
	else
	//ingame paused
	if (Game::gameState == 1 && isPaused == 1)
	{
		IngameMenu.getComponent<TransformComponent>().setPos(camera.x, camera.y);
		exitIngameButton.getComponent<TransformComponent>().setPos(camera.x, camera.y);
		soundIngameButton.getComponent<TransformComponent>().setPos(camera.x, camera.y + 156);

		if (exitIngameButton.getComponent<ButtonComponent>().alreadyPressed)
		{
			Game::isRunning = false;
		}

		if (soundIngameButton.getComponent<ButtonComponent>().alreadyPressed)
		{
			if (Game::playSound == true)
			{
				Game::playSound = false;
				soundIngameButton.getComponent<SpriteComponent>().setFrame(1);
			}
			else
			{
				Game::playSound = true;
				soundIngameButton.getComponent<SpriteComponent>().setFrame(0);
			}
		}
	}
	//starting menu
	if (Game::gameState == -1)
		{
			//button functionality

			if (playButton.getComponent<ButtonComponent>().alreadyPressed)
			{
				showTutorial = 0;
				Game::gameState = 1;

				playButton.destroy();
				tutorialButton.destroy();
				exitButton.destroy();
				danelButton.destroy();

				//load map and stuff

				map = new Map("assets/maptile_ss.png");

				map->LoadMapChunk("assets/fullmap.txt", 0, 0, 32, 32);
				map->SpawnEnemies();

				for (auto& e : enemies)
					e->getComponent<EnemyInfo>().player = &player;

				totalEnemies = enemies.size();

				//player stuff
				player.addComponent<TransformComponent>(1900, 850, 1.5f);
				player.addComponent<SpriteComponent>("assets/player.png", true);
				player.addComponent<KeyboardController>();
				player.addComponent<ColliderComponent>("player");
				player.addGroup(groupPlayers);

				//gun stuff
				gun.addComponent<TransformComponent>(1.5f);
				gun.addComponent<SpriteComponent>("assets/guns.png", false);
				gun.addGroup(groupPlayers);

				//UI stuff
				SDL_Color white = { 0, 0, 0, 255 };
				SDL_Color red = { 255, 0, 0, 255 };

				UIAmmo.addComponent<UILabel>(30, camera.h - 60, std::to_string(player.getComponent<KeyboardController>().ammo[player.getComponent<KeyboardController>().usedWeapon]), 30, white);
				UIAmmo.addGroup(groupUI);
				UIHP.addComponent<UILabel>(30, camera.h - 120, std::to_string(hp), 30, white);
				UIHP.addGroup(groupUI);

				//ingame menu 
				IngameMenu.addComponent<TransformComponent>(camera.x, camera.y, 341 * 1.1, 238 * 1.1, 1);
				IngameMenu.addComponent<SpriteComponent>("assets/menu.png", false);
				IngameMenu.addGroup(groupButtons);

				exitIngameButton.addComponent<TransformComponent>(camera.x, camera.y, 133, 238, 1);
				exitIngameButton.addComponent<SpriteComponent>("assets/ExitButton.png", false);
				exitIngameButton.addComponent<ButtonComponent>();
				exitIngameButton.addGroup(groupButtons);

				soundIngameButton.addComponent<TransformComponent>(camera.x, camera.y + 128, 85, 229, 1);
				soundIngameButton.addComponent<SpriteComponent>("assets/SoundButton.png", false);
				soundIngameButton.addComponent<ButtonComponent>();
				soundIngameButton.addGroup(groupButtons);
				

			}

			//more button stuff

			if (tutorialButton.getComponent<ButtonComponent>().alreadyPressed)
			{
				if (showTutorial == 0)
					showTutorial = 1;			
			}

			if (exitButton.getComponent<ButtonComponent>().alreadyPressed)
			{
				Game::isRunning = false;
				danelButton.destroy();
				playButton.destroy();
				tutorialButton.destroy();
				exitButton.destroy();
			}

			if (danelButton.getComponent<ButtonComponent>().alreadyPressed)
			{
				ShellExecute(NULL, NULL, "https://danelthedev.000webhostapp.com/", NULL, NULL, SW_SHOWNORMAL);
			}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	if (Game::gameState == -1)
		fundal.draw();

	for (auto& t : tiles) //draw map
	{
		t->draw();
	}

	for (auto& c : colliders) //draw colliders
	{
		c->draw();
	}

	for (auto& p : players) //draw player
	{
		p->draw();
	}

	for (auto& b : boolets) //draw boolet
	{
		b->draw();
	}

	for (auto& c : consumables)
	{
		c->draw();
	}

	for (auto& e : enemies) // draw enemies
	{
		e->draw();
	}

	for (auto& b : booletsE) //draw boolet
	{
		b->draw();
	}

	for (auto& u : UIs) // draw UI
	{
		u->draw();
	}

	for (auto& b : buttons)
	{
		b->draw();
	}


	if (showTutorial)
		tutorial.draw();

	if ((totalEnemies == 37 && currentLvl == 1) || (totalEnemies == 25 && currentLvl == 2))
	{
		levelComplete.getComponent<TransformComponent>().setPos(camera.x + 220, camera.y + 25);
		levelComplete.draw();
	}

	if (currentLvl == 4)
	{
		gameComplete.getComponent<TransformComponent>().setPos(camera.x + 220, camera.y + 250);
		gameComplete.draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	TTF_Quit();
	SDL_QUIT;

	std::cout << "Game cleaned" << std::endl;
}