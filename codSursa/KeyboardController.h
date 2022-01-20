#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <SDL_mixer.h>

class KeyboardController : public Component
{
public:

	Mix_Chunk* pew = NULL;

	TransformComponent* transform;
	SpriteComponent* sprite;

	int ownedWeapons[3] = { 1, 1, 1 }; //the guns you have access to
	int shootCurCDs[3] = { 0, 0, 0 };
	int shootCDs[3] = { 60, 90, 10 }; //individual cd of the weapons
	int usedWeapon = 1; //0 = pistol; 1 = shotgun; 2 = smg

	int magCurAmmo[3] = { 7, 2, 20 }; //current loaded ammo
	int magMaxAmmo[3] = { 7, 2, 20 }; //maximum loaded ammo
	int ammo[3] = { 35, 10, 60}; //max ammo of each weapon

	const Uint8* kbs = SDL_GetKeyboardState(NULL);
	int hdir = 0, vdir = 0;

	int shot = 0;

	//test vars
	int bShot = 0;


	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if(Game::isPaused == 0)
		{
			//MOUSE EVENTS

			//pozitia mouse
			int xMouse, yMouse;
			SDL_GetMouseState(&xMouse, &yMouse);
			float angle = atan2(yMouse - (transform->position.y - Game::camera.y + 32), xMouse - (transform->position.x - Game::camera.x + 32)) * 180 / 3.141;
			transform->rot = angle;


			//click apasat sau nu
			if (Game::event.button.state == SDL_PRESSED && shootCurCDs[usedWeapon] == 0 && magCurAmmo[usedWeapon] > 0)
			{
				magCurAmmo[usedWeapon] --;
				pew = Mix_LoadWAV("assets/pew.wav");
				if (Game::playSound == true)
				Mix_PlayChannel(-1, pew, 0);
				shot = 1;
				shootCurCDs[usedWeapon] = shootCDs[usedWeapon];

			}
			else { shot = 0; if (shootCurCDs[usedWeapon] > 0) shootCurCDs[usedWeapon]--; }
		
			//KEYBOARD EVENTS


			//change weapon
			if ((bool)kbs[SDL_SCANCODE_1] && ownedWeapons[0])
				usedWeapon = 0;

			if ((bool)kbs[SDL_SCANCODE_2] && ownedWeapons[1])
				usedWeapon = 1;

			if ((bool)kbs[SDL_SCANCODE_3] && ownedWeapons[2])
				usedWeapon = 2;

			//reload

			if ((bool)kbs[SDL_SCANCODE_R] && magCurAmmo[usedWeapon] != magMaxAmmo[usedWeapon])
			{
				if (ammo[usedWeapon] >= magMaxAmmo[usedWeapon])
				{
					ammo[usedWeapon] -= (magMaxAmmo[usedWeapon] - magCurAmmo[usedWeapon]);
					magCurAmmo[usedWeapon] = magMaxAmmo[usedWeapon];
				}
				else
					if (ammo[usedWeapon] < magMaxAmmo[usedWeapon])
					{
						magCurAmmo[usedWeapon] += ammo[usedWeapon];
						ammo[usedWeapon] = 0;
					}
			}

			//movement

			hdir = (bool)kbs[SDL_SCANCODE_D] - (bool)kbs[SDL_SCANCODE_A];
			vdir = (bool)kbs[SDL_SCANCODE_S] - (bool)kbs[SDL_SCANCODE_W];

			if (hdir || vdir) sprite->Play("Walk");

			if (hdir == 0 && vdir == 0)
			{
				transform->velocity.x = 0; 
				transform->velocity.y = 0; 
				sprite->Play("Idle");
			} 
			else if (hdir != 0 && vdir != 0)
			{
				transform->velocity.x = hdir * 0.71;
				transform->velocity.y = vdir * 0.71;
			}
			else
			{
				transform->velocity.x = hdir;
				transform->velocity.y = vdir;
			}
		}
		else
		{
			vdir = hdir = 0;
			transform->velocity.x = 0;
			transform->velocity.y = 0;
		}
	}
};