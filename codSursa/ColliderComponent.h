#pragma once
#include <string>
#include "Components.h"
#include "Texture.h"
#include <SDL.h>

class ColliderComponent : public Component
{
public:

	SDL_Rect collider; //collision box
	std::string tag;  //tag the ID of the collided object

	SDL_Texture* tex;
	SDL_Rect srcR, destR;


	TransformComponent* transform;
	
	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/ColTex.png");
		srcR = { 0, 0, 64, 64 };
		destR = {collider.x, collider.y, collider.w, collider.h};


	}

	void update() override
	{
		
		if(tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x); //seteaza coliderul la pozitia obiectului
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
		
		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		//TextureManager::Draw(tex, srcR, destR, 0); //to be commented on release
	}

};