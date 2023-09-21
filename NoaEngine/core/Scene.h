#ifndef NOAENGINE_SCENE_H
#define NOAENGINE_SCENE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>
#include <functional>
#include <list>
#include <map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Sprite.h"
#include "Resource.h"


using namespace std;

namespace noa {
	typedef unsigned char Uint8;

	class GameObject;
	class Actor;
	class Rigidbody;

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
		TileMap();
		TileMap(unordered_map<int,Tile*> tileSet,MapFile map);
		TileMap(unordered_map<int, Tile*> tileSet, vector<MapFile> mapLayer);
		int GetTileID(const int x,const int y) const;
		void SetTileID(const int x, const int y,const int tileID);
		//仅仅适用于静态的全局Tile，不使用于带动画的事件Tile
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
	};

	struct GameObjectBuffer;
	typedef struct GameObjectBuffer GameObjectBuffer;

	/// <summary>
	/// map,object,tile set
	/// </summary>
	typedef struct SceneInfo
	{
		string mapPath = "";
		string mapObjectPath = "";
		string mapTileSetPath = "";
	}SceneInfo;

	typedef struct MapInfo {
		TileMap mapLayer;
		MapFile objectLayer;
	}MapInfo;

	class Scene 
	{
	public:
		string name = "Scene";
		SceneInfo info;

		map<size_t, Rigidbody*> rigidbodys;
		map<size_t, Actor*> actors;//调用
		vector<GameObjectBuffer> gameObjects;//绘图用的

		vector<Rigidbody*> destroyRigids;
		vector<Actor*> destroyActors;

	public:
		Scene(string name, SceneInfo info);
		~Scene();

		MapInfo GetTileMap();

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void Unload() {}

	public:
		void AddActor(Actor* actor);
		void RemoveActor(Actor * actor);

		void AddGameObject(GameObjectBuffer gameObject);
		void RemoveGameObject(GameObject* gameObject);
		
		void AddRigidbody(Rigidbody* actor);
		void RemoveRigidbody(Rigidbody* actor);

		void ActorAwake();
		void ActorOnEnable();
		void ActorStart();
		void ActorUpdate();
		void ActorOnDisable();

		void DestoyScene();

	};

	class SceneManager 
	{
	public:
		//加载场景时发生的事件
		MapInfo activeMapInfo;
		Scene * GetActiveScene();
		void LoadScene(string sceneName);
		void AddScene(Scene* scene);

		void Awake();
		void Destroy();
		void Start();
		void Update();

	public:
		//保存当前场景的刚体和actors
		

	private:
		Scene* oldScene = nullptr;
		Scene * activeScene = nullptr;
		Scene* nextScene = nullptr;
		map<string, Scene*> sceneList;

		bool done = true;

	};

	extern SceneManager sceneManager;

}



#endif // !NOAENGINE_SCENE_H


