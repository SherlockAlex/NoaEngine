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

namespace noa {


	typedef unsigned char Uint8;

	class GameObject;
	class Actor;
	class Rigidbody;
	class ScriptableActor;
	class Camera;

	//关卡地图
	//注意图层的概念
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
		TileMap();
		TileMap(std::unordered_map<int,Tile> tileSet,MapFile map);
		TileMap(std::unordered_map<int, Tile> tileSet, std::vector<MapFile> mapLayer);
		virtual ~TileMap();

		int GetTileID(const int x,const int y) const;
		void SetTileID(const int x, const int y,const int tileID);
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
	};

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

	private:
		friend class SceneManager;

	private:
		std::string name = "Scene";
		std::vector<Actor*> actors;
		std::vector<Camera*> cameras;

		int mainCamera = -1;

	protected:
		Scene(std::string name);
		virtual ~Scene();

	public:
		void SetTileMap(TileMap * map);
		void Delete() { delete this; };
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void Unload() {}

	public:
		void AddCamera(Camera* camera);

		
		Camera* GetMainCamera();

		void AddActor(Actor* actor);
		void ActorUpdate();
		void DestoyScene();

		Actor* FindActorWithTag(const std::string& tag);
		std::vector<Actor*> FindActorsWithTag(const std::string& tag);

		std::string GetName();

	private:
		void ApplyCamera();

	};

	class SceneManager 
	{
	public:
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


