#include "Scene.h"
#include "NoaEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace noa 
{
	SceneManager sceneManager;

	MapFile LoadMap(const char* fileName)
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

	MapFile LoadMapFromCSV(const std::string filename)
	{
		Map map;

		std::ifstream file(filename);
		if (!file.is_open())
		{
			Debug("Failed to open file: "+filename);
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

	unordered_map<int, Tile*> LoadTileFromTsd(const std::string& fileName)
	{
		//加载游戏的tileSet

		unordered_map<int, Tile*> result;
		std::vector<PixelData> resultData;

		std::ifstream inputFile(fileName, std::ios::binary);
		if (inputFile)
		{
			int tileCount;
			inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

			Debug("tile set size:"+to_string(tileCount));

			for (int i = 0; i < tileCount; i++)
			{
				PixelData pixelData;
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

		for (PixelData data: resultData)
		{
			Tile* tile = new Tile(data.sprites);
			result[data.id] = tile;
		}

		Debug("Init tileset successfully");

		return result;
	}

	LevelMap::LevelMap() {

	}

	LevelMap::LevelMap(MapFile & map)
	{
		this->w = map.w;
		this->h = map.h;

		level = map.image;

		Debug("load map from file successfully");

	}

	void LevelMap::Construct(MapFile& map)
	{
		this->w = map.w;
		this->h = map.h;

		level = map.image;

		Debug("load map from file successfully");
	}

	void LevelMap::ConstructLayer(vector<vector<int>> layer)
	{
		levelLayer = layer;
		Debug("Update the layer map");
	}

	TileMap::TileMap(unordered_map<int, Tile*> tileSet, MapFile map) :LevelMap(map)
	{
		this->tileSet = tileSet;
	}

	TileMap::TileMap(unordered_map<int, Tile*> tileSet, vector<MapFile> mapLayer)
	{
		this->tileSet = tileSet;
		MapFile map;
		map.image = mapLayer[0].image;
		map.w = mapLayer[0].w;
		map.h = mapLayer[0].h;

		vector<vector<int>> layer;

		for (int i = 0;i<mapLayer.size();i++) 
		{
			layer.push_back(mapLayer[i].image);
			Debug("the layer is added");
		}

		for (int i=1;i<mapLayer.size();i++) 
		{
			for (int j = 0;j<mapLayer[i].image.size();j++)
			{
				if (mapLayer[i].image[j] == -1)
				{
					continue;
				}
				map.image[j] = mapLayer[i].image[j];
			}
		}

		this->Construct(map);
		this->ConstructLayer(layer);
	}

	int TileMap::GetTileID(const int x,const int y) const
	{
		if (x<0||x>=w||y<0||y>=h)
		{
			return -1;
		}
		return level[y*w+x];
	}

	bool TileMap::IsTile(int code) const
	{
		return ContainKey<int, Tile*>(tileSet, code);
	}

	Tile* TileMap::GetTile(int id)
	{
		if (!IsTile(id))
		{
			return nullptr;
		}
		return tileSet[id];
	}

	Scene::Scene(string name)
	{
		this->name = name;
		sceneManager.AddScene(this);
		this->OnEnable();
	}

	Scene::~Scene()
	{
		this->OnDisable();
	}

	Scene * SceneManager::GetActiveScene()
	{
		return activeScene;
	}

	void SceneManager::LoadScene(string sceneName)
	{
		if (ContainKey<string,Scene*>(this->sceneList,sceneName)) 
		{
			activeScene->OnDisable();
			activeScene = sceneList[sceneName];
			activeScene->OnEnable();
			Debug("Load scene:"+sceneName);
		}
	}

	void SceneManager::AddScene(Scene* scene)
	{
		if (scene == nullptr)
		{
			return;
		}
		if (activeScene==nullptr) 
		{
			activeScene = scene;
		}

		sceneList[scene->name] = scene;

	}

}