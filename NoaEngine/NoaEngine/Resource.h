#ifndef NOAENGINE_RESOURCE_H
#define NOAENGINE_RESOURCE_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

namespace noa {
	//��Դ����ϵͳ
	
	//���صľ�����Դ�ļ�
	typedef struct SpriteFile
	{
		std::vector<uint32_t> images;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	}SpriteFile;

	//���ض����ļ�
	typedef struct AnimationFile {
		std::vector<SpriteFile> data;
		int posx = 0;
		int posy = 0;
		int w = 0;
		int h = 0;
	}AnimationFile;

	//���ص�ͼ�ļ�
	typedef struct LayerFile
	{
		//��ͼ�ļ�
		int w = 0;
		int h = 0;
		std::vector<int> image;
	}LayerFile;

	
	class Sprite;
	class SpriteGPU;
	//��Ƭ
	typedef struct Tile
	{
		std::shared_ptr<Sprite> sprite = nullptr;
		bool isCollision = false;

		//���һ��Animation

		Tile();

		Tile(std::shared_ptr<Sprite> sprite, bool isCollision = false);

		Tile(SpriteFile spriteFile, bool isCollision = false);

		~Tile();


	}Tile;

	//��Ƭ����
	typedef struct TileData 
	{
		int id = -1;
		SpriteFile sprites;
	}TileData;

	typedef struct TileSet {
		std::unordered_map<int, Tile> set;
		bool ContainTileID(int id) const;
		Tile* GetTileByID(int id);

	}TileSet;

	class AudioClip;
	class Resource {
	public:
		static const std::string path;
	public:
		static std::shared_ptr<AudioClip> LoadAudioClip(const std::string & filename);

		static SpriteFile LoadSprite(const std::string & filename);

		static AnimationFile LoadAnimationClip(const std::string & filename);

		static LayerFile LoadMapLayer(const std::string & filename);

		static TileSet LoadTileSet(const std::string& filename);

		static std::string ReadTextFrom(const std::string & filename);

	};

}

#endif // !NOAENGINE_RESOURCE_H


