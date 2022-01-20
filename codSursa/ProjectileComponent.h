#pragma once

#include "Components.h"
#include <math.h>


class ProjectileComponent : public Component
{
public:
	TransformComponent* trans = nullptr;
	int rot = 0;
	int dmg = 50;

	ProjectileComponent() = default;

	ProjectileComponent(int r) { rot = r; }
	ProjectileComponent(int r, int d) { rot = r; dmg = d; }


	void init() override
	{
		trans = &entity->getComponent<TransformComponent>();
		trans->speed = 25;
		trans->rot = rot;
	}

	void update() override
	{
		trans->position.y += sin(rot * 3.14 / 180) * trans->speed;
		trans->position.x += cos(rot * 3.14 / 180) * trans->speed;
	}

};