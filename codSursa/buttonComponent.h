#pragma once

#include "Components.h"

class ButtonComponent : public Component
{
public:

	TransformComponent* transform;
	int mouseX, mouseY;
	bool alreadyPressed = false;

	int startx, starty;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		startx = transform->position.x;
		starty = transform->position.y;
	}

	void update() override
	{
		used();
	}

	bool used()
	{
		int actualMx, actualMy;
		SDL_GetMouseState(&mouseX, &mouseY);
		actualMx = mouseX + Game::camera.x;
		actualMy = mouseY + Game::camera.y;
		if ((actualMx > transform->position.x && actualMx < transform->position.x + transform->width * transform->scale)
			&& (actualMy > transform->position.y && actualMy < transform->position.y + transform->height * transform->scale))
		{
			transform->scale = 1.1;

			transform->position.x = startx - 0.05 * transform->width;
				transform->position.y = starty - 0.05 * transform->height;


			if (Game::event.button.state == SDL_PRESSED && alreadyPressed == 0)
			{
				alreadyPressed = true;	
				return true;
			}
		}
		else
		{
			transform->scale = 1;
			transform->position.x = startx;
			transform->position.y = starty;

			return false;
		}

		if (Game::event.button.state == SDL_RELEASED)
			alreadyPressed = false;
	}


};