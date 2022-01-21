#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;

	int height = 64, width = 64;
	float scale = 1;
	int rot = 0;

	int speed = 3;
	
	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc)
	{
		position.x = 0;
		position.y = 0;

		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, float sc)
	{
		position.x = x;
		position.y = y;
		scale = sc;
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	TransformComponent(TransformComponent* tc)
	{
		this->position.x = tc->position.x;
		this->position.y = tc->position.y;
		this->width = tc->width;
		this->height = tc->height;
		this->scale = tc->scale;
		this->rot = tc->rot;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	void copyTransform(TransformComponent* tc)
	{
		this->position.x = tc->position.x;
		this->position.y = tc->position.y;
		this->width = tc->width;
		this->height = tc->height;
		this->scale = tc->scale;
		this->rot = tc->rot;
	}

	void setPos(int x, int y)
	{
		this->position.x = x;
		this->position.y = y;
	}
	
	void setPos(int x, int y, int rot)
	{
		this->position.x = x;
		this->position.y = y;
		this->rot = rot;
	}
};
