#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "ProjectileComponent.h"
#include <math.h>
#include <time.h>

extern Manager manager;


class EnemyInfo : public Component
{
public:

	int hp = 100;
	int usedWeapon = 0; //0 = pistol, 1 = shotgun, 2 = smg
	int bShot = 0;
	
	ColliderComponent* col;
	TransformComponent* transform;
	SpriteComponent* sprite;

	Entity* player;

	int timer = 0;
	int ang = 0;
	
	bool canShoot = false;

	EnemyInfo() = default;
	
	EnemyInfo(Entity* p)
	{
		player = p;
	}
	

	void init() override
	{
		//srand(time(NULL));
		bShot = 0;

		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		col = &entity->getComponent<ColliderComponent>();

	}

	void update() override
	{
		if(Game::isPaused == 0)
		{
			followPoint();

			if (timer == 0 && getDist() < 500)
			{
				if (usedWeapon == 0)
				{
					ang = transform->rot + rand() % 10 - 5;
					auto& boolet(manager.addEntity());
					boolet.addComponent<TransformComponent>(transform->position.x + 32, transform->position.y + 32, 0.25);
					boolet.getComponent<TransformComponent>().rot = ang;
					boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
					boolet.addComponent<ProjectileComponent>(ang, 10);
					boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
						boolet.getComponent<TransformComponent>().position.y, 16);
					boolet.addGroup(Game::groupBooletsE);

					timer = 200;
				}
				else
				if(usedWeapon == 1)
				{
					ang = transform->rot + rand() % 10 - 5;
					for(int i = 0; i < 8; ++i)
					{
						auto& boolet(manager.addEntity());
						boolet.addComponent<TransformComponent>(transform->position.x + 32, transform->position.y + 32, 0.25);
						boolet.getComponent<TransformComponent>().rot = (ang - 60) + i * 15;
						boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
						boolet.addComponent<ProjectileComponent>((ang - 60) + i * 15, 10);
						boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
							boolet.getComponent<TransformComponent>().position.y, 16);
						boolet.addGroup(Game::groupBooletsE);
					}
					timer = 300;
				}
				else
					if (usedWeapon == 2)
					{
						ang = transform->rot + rand() % 20 - 10;
						auto& boolet(manager.addEntity());
						boolet.addComponent<TransformComponent>(transform->position.x + 32, transform->position.y + 32, 0.25);
						boolet.getComponent<TransformComponent>().rot = ang;
						boolet.addComponent<SpriteComponent>("assets/boolet.png", false);
						boolet.addComponent<ProjectileComponent>(ang, 10);
						boolet.addComponent<ColliderComponent>("boolets", boolet.getComponent<TransformComponent>().position.x,
							boolet.getComponent<TransformComponent>().position.y, 16);
						boolet.addGroup(Game::groupBooletsE);

						if (bShot >= 3)
						{
							timer = 300;
							bShot = 0;
						}
						else
						{
							timer = 10;
							bShot++;
						}
					
				}
			}
			if (timer > 0) timer--;
		}
	}

	int getDist()
	{
		return sqrt((player->getComponent<TransformComponent>().position.x - transform->position.x)
				* (player->getComponent<TransformComponent>().position.x - transform->position.x)
				+ (player->getComponent<TransformComponent>().position.y - transform->position.y)
				* (player->getComponent<TransformComponent>().position.y - transform->position.y)
		);
	}

	void followPoint()
	{
		//orientation

		if(getDist() < 500)
		{
			Vector2D playerPos = player->getComponent<TransformComponent>().position;

			float angle = atan2(playerPos.y - transform->position.y, playerPos.x - transform->position.x) * 180 / 3.141;
			transform->rot = angle;
		}
	}

};