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
#include "Resource.h"
#include <list>

using namespace std;

namespace noa {
	typedef unsigned char Uint8;

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
	class TileMap:public LevelMap
	{
	public:
		unordered_map<int, Tile*> tileSet;
		unordered_map<int, bool> collisionTiles;
	public:
		/// <summary>
		/// 加载瓦片地图
		/// </summary>
		/// <param name="tileSet">瓦片集合</param>
		/// <param name="map">地图数据</param>
		TileMap(unordered_map<int,Tile*> tileSet,MapFile map);
		TileMap(unordered_map<int, Tile*> tileSet, vector<MapFile> mapLayer);
		int GetTileID(const int x,const int y) const;
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
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


