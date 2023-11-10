#include "ResourceManager.h"
#include "Debug.h"
#include <fstream>

std::string ResourceManager::path = "./";

SpriteFile ResourceManager::LoadSprite(const std::string& filename)
{
	SpriteFile spriteFile;

	const std::string filePath = ResourceManager::path + filename;

	std::ifstream input(filePath, std::ios::binary);
	if (input)
	{
		uint32_t imagesCount;
		input.read(reinterpret_cast<char*>(&imagesCount), sizeof(uint32_t));

		for (uint32_t i = 0; i < imagesCount; ++i)
		{
			uint32_t imageValue;
			input.read(reinterpret_cast<char*>(&imageValue), sizeof(uint32_t));
			spriteFile.images.push_back(imageValue);
		}

		input.read(reinterpret_cast<char*>(&spriteFile.x), sizeof(int));
		input.read(reinterpret_cast<char*>(&spriteFile.y), sizeof(int));
		input.read(reinterpret_cast<char*>(&spriteFile.width), sizeof(int));
		input.read(reinterpret_cast<char*>(&spriteFile.height), sizeof(int));

		input.close();
	}
	else
	{
		noa::Debug::Warring("∂¡»° ß∞‹£∫" + std::string(filename));
	}

	return spriteFile;
}
