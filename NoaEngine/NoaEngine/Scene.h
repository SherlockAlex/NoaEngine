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
#include "Resource.h"
#include "Debug.h"

namespace noa {


	typedef unsigned char Uint8;

	class Actor;
	class Rigidbody;
	class ScriptableActor;
	class Camera;

	//¹Ø¿¨µØÍ¼
	//×¢ÒâÍ¼²ãµÄ¸ÅÄî
	class LevelMap {
	public:
		std::vector<int> level;
		std::vector<std::vector<int>> levelLayer;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(const MapFile & map);
		void Construct(const MapFile& map);
		void ConstructLayer(const std::vector<std::vector<int>> & layer);
		
	};

	//ÍßÆ¬µØÍ¼
	class TileMap:public LevelMap
	{
	public:
		std::unordered_map<int, Tile> tileSet;
		std::unordered_map<int, bool> collisionTiles;
	public:
		TileMap();
		TileMap(const std::unordered_map<int,Tile> & tileSet,const MapFile & map);
		TileMap(const std::unordered_map<int,Tile> & tileSet,const std::vector<MapFile> & mapLayer);
		virtual ~TileMap();

		int GetTileID(const int x,const int y) const;
		void SetTileID(const int x, const int y,const int tileID);
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(const std::vector<int> & collisionTileIDs);
	};

	typedef struct SceneInfo
	{
		std::string mapPath = "";
		std::string mapObjectPath = "";
		std::string mapTileSetPath = "";
	}SceneInfo;

	typedef struct MapInfo {
		std::shared_ptr<TileMap> mapLayer;
		std::shared_ptr<MapFile> objectLayer;

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
		Scene(const std::string & name);
		virtual ~Scene();

	public:
		void SetTileMap(TileMap * map);
		
	protected:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void Unload() {}

	public:
		void AddCamera(Camera* camera);
		Camera* GetMainCamera();

		void AddActor(Actor* actor);

		std::string GetName();

	private:

		Actor* FindActorWithTag(const std::string& tag);
		std::vector<Actor*> FindActorsWithTag(const std::string& tag);

	private:
		void ApplyCamera();

		void ActorUpdate();
		void DestoyScene();

		void Delete();

	};

	class SceneManager 
	{

	private:
		friend class NoaEngine;
		friend class Scene;

	public:
		Scene * GetActiveScene();
		void LoadScene(const std::string & sceneName);

		~SceneManager();

		void Quit();

		template<class T>
		std::vector<T*> FindActorsWithType() {
			std::vector<T*> results;
			if (this->activeScene == nullptr)
			{
				return results;
			}

			for (auto & actor:this->activeScene->actors) 
			{
				if (actor == nullptr
					||!actor->GetActive()
					||actor->isRemoved
					)
				{
					continue;
				}
				T* buffer = dynamic_cast<T*>(actor);
				if (buffer!=nullptr)
				{
					results.push_back(buffer);
				}
			}
			return results;

		}

		template<class T>
		T* FindActorWithType() {
			if (this->activeScene == nullptr) 
			{
				return nullptr;
			}
			T* buffer = nullptr;
			for (auto& actor : this->activeScene->actors)
			{
				if (actor == nullptr
					|| !actor->GetActive()
					|| actor->isRemoved)
				{
					continue;
				}
				buffer = dynamic_cast<T*>(actor);
				if (buffer != nullptr)
				{
					break;
				}
			}
			return buffer;
		}

		Actor* FindActorWithTag(const std::string& tag);

		std::vector<Actor*> FindActorsWithTag(const std::string& tag);
		
	private:
		void Awake();
		void Destroy();
		void Start();
		void Update();

		void AddScene(Scene* scene);

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


