#ifndef NOAENGINE_SCENE_H
#define NOAENGINE_SCENE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Sprite.h"
#include <list>

using namespace std;

namespace noa {
	typedef unsigned char Uint8;

	typedef struct MapFile
	{
		//地图文件
		int w = 0;
		int h = 0;
		vector<int> image;
	}Map;


	//加载地图
	extern MapFile LoadMap(const char* fileName);

	extern MapFile LoadMapFromCSV(const std::string filename);

	class LevelMap {
	public:
		vector<int> level;
		vector<vector<int>> levelLayer;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(MapFile & map);
		void Construct(MapFile& map);
		void ConstructLayer(vector<vector<int>> layer);
		
	};

	//瓦片地图
	typedef struct Tile 
	{
		Sprite * sprite = nullptr;
		bool isCollision = false;

		Tile() 
		{

		}

		Tile(Sprite * sprite,bool isCollision = false) 
		{
			this->sprite = sprite;
			this->isCollision = isCollision;
		}

		Tile(SpriteFile spriteFile, bool isCollision = false)
		{
			this->sprite =new Sprite(spriteFile,Vector<int>(1.0,1.0));
			this->isCollision = isCollision;
		}

	}Tile;

	typedef struct PixelData {
		int id;
		SpriteFile sprites;
	}PixelData;

	//extern vector<PixelData> LoadTileFromTsd(const std::string& fileName);

	extern unordered_map<int, Tile*> LoadTileFromTsd(const std::string& fileName);

	//瓦片地图
	class TileMap:public LevelMap
	{
	public:
		unordered_map<int, Tile*> tileSet;
	public:
		/// <summary>
		/// 加载瓦片地图
		/// </summary>
		/// <param name="tileSet">瓦片集合</param>
		/// <param name="map">地图数据</param>
		TileMap(unordered_map<int,Tile*> tileSet,MapFile map);
		TileMap(unordered_map<int, Tile*> tileSet, vector<MapFile> mapLayer);
		int GetTileID(const int x,const int y) const;
		Tile* GetTile(int id);
		bool IsTile(int code) const;
	};

	class Scene 
	{
	public:
		string name = "Scene";
		virtual void OnEnable() {};
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnDisable() {};

	public:
		Scene(string name);
		~Scene();
	};

	class SceneManager 
	{
	public:
		Scene * GetActiveScene();
		void LoadScene(string sceneName);
		void AddScene(Scene* scene);
	private:
		Scene * activeScene = nullptr;
		unordered_map<string, Scene*> sceneList;
	};

	extern SceneManager sceneManager;

}



#endif // !NOAENGINE_SCENE_H


