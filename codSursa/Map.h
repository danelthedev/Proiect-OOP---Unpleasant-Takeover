#pragma once
#include <string>

class Map
{
public:
	Map(const char* mfp);
	~Map();

	void LoadMapChunk(std::string path, int startX, int startY, int finishX, int finishY);
	void SpawnEnemies();
	void AddTile(int srcX, int srcY, int xpos, int ypos);

	static int sizeX, sizeY;

private:
	const char* mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};