#include "Resource.h"
#include "Sprite.h"
#include "Debug.h"
#include "AudioSystem.h"

using namespace std;

const std::string noa::Resource::path =
	"./Data/Resource/assets/";

bool noa::TileSet::ContainTileID(int id) const
{
	return this->set.count(id) > 0;
}

noa::Tile* noa::TileSet::GetTileByID(int id)
{
	if (this->set.count(id)<=0) 
	{
		return nullptr;
	}
	return &this->set[id];
}

std::shared_ptr<noa::AudioClip> noa::Resource::LoadAudioClip(const std::string & filename) 
{
	const std::string filePath = Resource::path + filename;
	return noa::audioSystem->LoadAudioClip(filePath);
}

noa::SpriteFile noa::Resource::LoadSprite(const std::string& filename)
{
	SpriteFile spriteFile;

	const std::string filePath = Resource::path + filename;

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
		Debug::Warring("读取失败：" + string(filename));
	}

	return spriteFile;
}

noa::AnimationFile noa::Resource::LoadAnimationClip(const std::string& filename)
{
	AnimationFile animator;
	const std::string filePath = Resource::path + filename;
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		Debug::Error("Opening file: " + string(filename) + " error");
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

noa::LayerFile noa::Resource::LoadMapLayer(const std::string& filename)
{
	LayerFile map;
	const std::string filePath = Resource::path + filename;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		Debug::Warring("Failed to open file: " + filename);
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

noa::TileSet noa::Resource::LoadTileSet(const std::string& filename)
{
	//加载游戏的tileSet
	noa::TileSet tileSet;
	//unordered_map<int, Tile> result;
	std::vector<TileData> resultData;
	const std::string filePath = Resource::path + filename;
	std::ifstream inputFile(filePath, std::ios::binary);
	if (inputFile)
	{
		int tileCount;
		inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

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

	for (const auto& data : resultData)
	{
		Tile tile(data.sprites);
		tileSet.set[data.id] = std::move(tile);
	}

	return tileSet;
}

std::string noa::Resource::ReadTextFrom(const std::string& filename)
{
	const std::string filePath = Resource::path + filename;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return std::string();
	}
	std::string sourceCode = std::string(
		std::istreambuf_iterator<char>(file)
		, (std::istreambuf_iterator<char>())
	);
	return sourceCode;
}

noa::Tile::Tile()
{

}

noa::Tile::Tile(shared_ptr<Sprite> sprite, bool isCollision)
{
	this->sprite = sprite;
	this->isCollision = isCollision;
}

noa::Tile::Tile(SpriteFile spriteFile, bool isCollision)
{
	this->sprite = make_shared<Sprite>(spriteFile, Vector<int>(1, 1));
	this->isCollision = isCollision;
}

noa::Tile::~Tile()
{

}
