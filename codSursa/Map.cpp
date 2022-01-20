#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(const char* mfp) : mapFilePath(mfp)
{
	srand(time(NULL));
}

Map::~Map()
{
	auto& tiles(manager.getGroup(Game::groupMap));

	for (auto& t : tiles)
	{
		(*t).destroy();
	}
}

int Map::sizeX = 0, Map::sizeY = 0;


void Map::LoadMapChunk(std::string path, int startX, int startY, int finishX, int finishY)
{
	char c;
	std::ifstream mapFile;
	mapFile.open(path);


	mapFile >> Map::sizeX >> Map::sizeY;
	mapFile >> tileSize;
	scaledSize = tileSize;

	mapFile.ignore();

	int srcX, srcY;

	for (int y = 0; y < Map::sizeY; y++)
	{
		for (int x = 0; x < Map::sizeX; x++)
		{
			mapFile.get(c);

			if(y >= startY && y <= finishY && x >= startX && x <= finishX)
			{

				srcY = 0;
				srcX = atoi(&c) * tileSize;
				AddTile(srcX, srcY, x * tileSize, y * tileSize);
			}
			mapFile.ignore();
		}
	}


	mapFile.ignore();

	for (int y = 0; y < Map::sizeY; y++)
	{
		for (int x = 0; x < Map::sizeX; x++)
		{
			mapFile.get(c);

			if (y >= startY && y <= finishY && x >= startX && x <= finishX)
			{
				if (c == '1' || c == '4')
				{
					auto& tcol(manager.addEntity());
					tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
					tcol.addGroup(Game::groupColliders);
				}
			}
			mapFile.ignore();
		}
	}

	mapFile.ignore();

	for (int y = 0; y < Map::sizeY; y++)
	{
		for (int x = 0; x < Map::sizeX; x++)
		{
			mapFile.get(c);


			if (y >= startY && y <= finishY && x >= startX && x <= finishX)
			{
				if (c != '0')
				{
					auto& item(manager.addEntity());
					item.addComponent<TransformComponent>(x * 64, y * 64, 64, 64, 1.5);
					item.addComponent<SpriteComponent>("assets/consumables.png", false);

					switch (c)
					{
					case '7':
						item.getComponent<SpriteComponent>().setFrame(0);
						item.addComponent<ColliderComponent>("medkit");
						break;
					case '8':
						item.getComponent<SpriteComponent>().setFrame(1);
						item.addComponent<ColliderComponent>("ammobag");
						break;
					}
					item.addGroup(Game::groupConsumables);
				}
			}

			mapFile.ignore();
		}
	}


	mapFile.close();
}


void Map::SpawnEnemies()
{
	char c;
	std::ifstream mapFile;
	mapFile.open("assets/inamici.txt");

	for (int y = 0; y < Map::sizeY; y++)
	{
		for (int x = 0; x < Map::sizeX; x++)
		{
			mapFile.get(c); //std::cout << c << ' ';

			if (c == '1')
			{
				auto& enemy(manager.addEntity());
				enemy.addComponent<TransformComponent>(x * 64, y * 64, 64, 64, 1.5);
				enemy.addComponent<ColliderComponent>("enemy", x * scaledSize, y * scaledSize, scaledSize);
				enemy.addComponent<EnemyInfo>();
				enemy.getComponent<EnemyInfo>().usedWeapon = rand() % 3;
				Vector2D tmp(x * 64, y * 64);
				Game::enemycoords.emplace_back(tmp);

				switch (enemy.getComponent<EnemyInfo>().usedWeapon)
				{
				case 0:
					enemy.addComponent<SpriteComponent>("assets/enemyPistol.png", false);
					break;
				case 1:
					enemy.addComponent<SpriteComponent>("assets/enemyShotgun.png", false);
					break;
				case 2:
					enemy.addComponent<SpriteComponent>("assets/enemySMG.png", false);
					break;
				}


				enemy.addGroup(Game::groupEnemies);
				
			}

			mapFile.ignore();
		}

	}

	mapFile.close();

}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapFilePath);
	tile.addGroup(Game::groupMap);

}
