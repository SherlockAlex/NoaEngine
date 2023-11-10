#pragma once
#include <vector>
#include <string>


typedef struct SpriteFile
{
	std::vector<uint32_t> images;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
}SpriteFile;

class ResourceManager
{
private:
	static std::string path;
public:
	static SpriteFile LoadSprite(const std::string& filename);
};

