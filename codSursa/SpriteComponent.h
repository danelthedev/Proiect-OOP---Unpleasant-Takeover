#pragma once

#include "Components.h"
#include "Texture.h"
#include "Animation.h"
#include <map>
#include <SDL.h>

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	int frames = 0;
	int speed = 100; //delay in ms
	bool animated = false;

public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SpriteComponent() = default;

	SpriteComponent(const char* path)
	{
		setTex(path);
	}

	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 2, 300);
		Animation walk = Animation(1, 4, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");

		setTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, transform->rot);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

	void setFrame(int f)
	{
		srcRect.x = srcRect.w * f;
	}

	int getFrame()
	{
		return srcRect.x / srcRect.w;
	}

	void setAnimSet(int as)
	{
		srcRect.y = srcRect.h * as;
	}

	void startAnim()
	{
		animated = 1;
	}

	void stopAnim()
	{
		animated = 0;
		srcRect.x = 0;
	}

	bool animEnd()
	{
		if (static_cast<int>((SDL_GetTicks() / speed) % frames) == 0)
		{
			std::cout << "Animation ended\n";
			return true;
		}
		return false;
	}

};