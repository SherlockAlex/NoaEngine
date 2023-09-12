#include "Resource.h"
#include "NoaEngine.h"
#include "Sprite.h"

namespace noa {

	Resource resource;

	SpriteFile Resource::LoadSprFile(const char* file) const
	{
		SpriteFile spriteFile;
		//spriteFile.images = nullptr;

		std::ifstream input(file, std::ios::binary);
		if (input)
		{
			uint32_t imagesCount;
			input.read(reinterpret_cast<char*>(&imagesCount), sizeof(uint32_t));

			//spriteFile.images = new uint32_t[imagesCount];
			for (int i = 0; i < imagesCount; ++i)
			{
				uint32_t imageValue;
				input.read(reinterpret_cast<char*>(&imageValue), sizeof(uint32_t));
				spriteFile.images.push_back(imageValue);
				//input.read(reinterpret_cast<char*>(&spriteFile.images[i]), sizeof(uint32_t));
			}

			input.read(reinterpret_cast<char*>(&spriteFile.x), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.y), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.width), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.height), sizeof(int));

			input.close();
		}
		else
		{
			Debug("读取失败：" + string(file));
			//std::cerr << "读取失败：" << file << std::endl;
		}

		return spriteFile;
	}

	AnimationFile Resource::LoadAnimationFile(const char* file) const
	{
		AnimationFile animator;

		std::ifstream inputFile(file, std::ios::binary);
		if (!inputFile) {
			Debug("Opening file: " + string(file) + " error");
			exit(-1);
		}

		int spriteCount;
		inputFile.read(reinterpret_cast<char*>(&spriteCount), sizeof(int));

		for (int i = 0; i < spriteCount; ++i) {
			SpriteFile sprite;
			int imageCount;
			inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

			for (int j = 0; j < imageCount; ++j) {
				uint32_t pixel;
				inputFile.read(reinterpret_cast<char*>(&pixel), sizeof(uint32_t));
				sprite.images.push_back(pixel);
			}

			inputFile.read(reinterpret_cast<char*>(&sprite.x), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.y), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.width), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.height), sizeof(int));

			animator.data.push_back(sprite);
		}

		inputFile.read(reinterpret_cast<char*>(&animator.posx), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.posy), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.w), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.h), sizeof(int));

		inputFile.close();
		return animator;
	}

	MapFile Resource::LoadMap(const char* fileName) const
	{

		MapFile map;

		// 打开二进制文件
		std::ifstream file(fileName, std::ios::binary);
		if (!file)
		{
			std::cout << "打开文件失败" << std::endl;
			return map;
		}

		// 读取地图宽度和高度
		file.read(reinterpret_cast<char*>(&map.w), sizeof(int));
		file.read(reinterpret_cast<char*>(&map.h), sizeof(int));

		// 计算图像的总像素数
		int imageSize = map.w * map.h;

		// 读取图像灰度值
		map.image.resize(imageSize);
		file.read(reinterpret_cast<char*>(&map.image[0]), imageSize * sizeof(int));

		// 关闭文件
		file.close();

		return map;

	}

	MapFile Resource::LoadMapFromCSV(const std::string filename) const
	{
		Map map;

		std::ifstream file(filename);
		if (!file.is_open())
		{
			Debug("Failed to open file: " + filename);
			//std::cout << "Failed to open file: " << filename << std::endl;
			return map;
		}

		std::string line;
		int row = 0;
		int col = 0;

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string cell;

			while (std::getline(ss, cell, ','))
			{
				int value = std::stoi(cell);
				map.image.push_back(value);
				col++;
			}

			if (row == 0)
			{
				map.w = col;
			}
			else if (col != map.w)
			{
				Debug("Invalid row width at line: " + to_string(row));
				//std::cout << "Invalid row width at line: " << row << std::endl;
				map.image.clear();
				break;
			}

			col = 0;
			row++;
		}

		map.h = row;

		file.close();

		return map;
	}

	unordered_map<int, Tile*> Resource::LoadTileFromTsd(const std::string& fileName) const
	{
		//加载游戏的tileSet

		unordered_map<int, Tile*> result;
		std::vector<TileData> resultData;

		std::ifstream inputFile(fileName, std::ios::binary);
		if (inputFile)
		{
			int tileCount;
			inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

			Debug("tile set size:" + to_string(tileCount));

			for (int i = 0; i < tileCount; i++)
			{
				TileData pixelData;
				inputFile.read(reinterpret_cast<char*>(&pixelData.id), sizeof(int));

				int imageCount;
				inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.x), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.y), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.width), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.height), sizeof(int));

				for (int j = 0; j < imageCount; j++)
				{
					int pixelValue;
					inputFile.read(reinterpret_cast<char*>(&pixelValue), sizeof(int));
					pixelData.sprites.images.push_back(pixelValue);
				}

				resultData.push_back(pixelData);
			}

			inputFile.close();
		}

		for (TileData data : resultData)
		{
			Tile* tile = new Tile(data.sprites);
			result[data.id] = tile;
		}

		Debug("Init tileset successfully");

		return result;
	}

	Tile::Tile()
	{

	}

	Tile::Tile(Sprite* sprite, bool isCollision)
	{
		this->sprite = sprite;
		this->isCollision = isCollision;
	}

	Tile::Tile(SpriteFile spriteFile, bool isCollision)
	{
		this->sprite = new Sprite(spriteFile, Vector<int>(1.0, 1.0));
		this->isCollision = isCollision;
	}

}