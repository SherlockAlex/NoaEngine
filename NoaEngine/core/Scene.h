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

#include "GameObject.h"

namespace noa {
	/*struct GameObjectBuffer;
	typedef struct GameObjectBuffer GameObjectBuffer;*/

	typedef unsigned char Uint8;

	class GameObject;
	class Actor;
	class Rigidbody;
	class ScriptableActor;

	class LevelMap {
	public:
		std::vector<int> level;
		std::vector<std::vector<int>> levelLayer;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(MapFile & map);
		void Construct(MapFile& map);
		void ConstructLayer(std::vector<std::vector<int>> layer);
		
	};

	//瓦片地图
	class TileMap:public LevelMap
	{
	public:
		std::unordered_map<int, Tile> tileSet;
		std::unordered_map<int, bool> collisionTiles;
	public:
		/// <summary>
		/// 加载瓦片地图
		/// </summary>
		/// <param name="tileSet">瓦片集合</param>
		/// <param name="map">地图数据</param>
		TileMap();
		TileMap(std::unordered_map<int,Tile> tileSet,MapFile map);
		TileMap(std::unordered_map<int, Tile> tileSet, std::vector<MapFile> mapLayer);
		virtual ~TileMap();

		int GetTileID(const int x,const int y) const;
		void SetTileID(const int x, const int y,const int tileID);
		//仅仅适用于静态的全局Tile，不使用于带动画的事件Tile
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
	};

	

	/// <summary>
	/// map,object,tile set
	/// </summary>
	typedef struct SceneInfo
	{
		std::string mapPath = "";
		std::string mapObjectPath = "";
		std::string mapTileSetPath = "";
	}SceneInfo;

	typedef struct MapInfo {
		TileMap * mapLayer;
		MapFile * objectLayer;

		~MapInfo() {
			if (mapLayer!=nullptr)
			{
				delete mapLayer;
			}

			if (objectLayer!=nullptr)
			{
				delete objectLayer;
			}

		}

	}MapInfo;

	class Scene 
	{
	public:
		std::string name = "Scene";
		//SceneInfo info;

		std::map<size_t, Rigidbody*> rigidbodys;
		std::map<size_t, Actor*> actors;//调用
		std::vector<GameObjectBuffer> gameObjects;//绘图用的

		std::vector<Rigidbody*> destroyRigids;
		std::vector<Actor*> destroyActors;

		std::vector<ScriptableActor*> destroyScriptableActors;

	protected:
		Scene(std::string name);
		virtual ~Scene();

	public:
		virtual void Delete() = 0;

		//MapInfo GetTileMap();

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

		void AddScriptableActor(ScriptableActor * SA);
		void ClearSA();


	};

	class SceneManager 
	{
	public:
		//加载场景时发生的事件
		//MapInfo activeMapInfo;
		Scene * GetActiveScene();
		void LoadScene(std::string sceneName);
		void AddScene(Scene* scene);

		void Awake();
		void Destroy();
		void Start();
		void Update();

		~SceneManager();

		void Quit();


	public:
		//保存当前场景的刚体和actors
		

	private:
		Scene* oldScene = nullptr;
		Scene * activeScene = nullptr;
		Scene* nextScene = nullptr;
		std::map<std::string, Scene*> sceneList;

		bool done = true;
		bool isLoading = false;
		
	public:
		bool isQuit = false;
		bool quited = false;

	};

	extern SceneManager sceneManager;

}



#endif // !NOAENGINE_SCENE_H


