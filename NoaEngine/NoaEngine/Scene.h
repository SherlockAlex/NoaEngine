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
#include "NoaAction.h"
#include "Resource.h"
#include "Debug.h"
#include "NObject.h"

namespace noa {


	typedef unsigned char Uint8;

	class Actor;
	class Rigidbody;
	class ScriptableActor;
	class Camera;

	//地图的层级
	class MapLayer 
	{
	public:
		std::vector<int> layer;
		uint32_t w = 0;
		uint32_t h = 0;
	public:
		MapLayer(const LayerFile & layer);
		virtual ~MapLayer();

	public:
		int GetTileID(int x, int y) const;
		void SetTileID(int x,int y,int value);
		
	};

	//关卡地图
	//一张地图是由许多的地图层组成的
	class Level 
	{
	public:
		std::vector<MapLayer> layers;
		uint32_t w = 0;
		uint32_t h = 0;
	protected:
		Level(Scene* scene);
		virtual ~Level();


		void Delete(Level*& level);
		void LoadLayer(const std::vector<std::string>& file);
		void Construct(const std::vector<MapLayer>& layer);
	public:

		template<class T>
		T* GetLevelAs() {
			return dynamic_cast<T*>(this);
		}

	private:
		friend class Scene;
		Scene* scene = nullptr;
	};
	
	class Scene;

	//瓦片地图
	//一个相机可以同时绘制多张TileMap
	//同时TileMap是有自己的Transform
	//瓦片的坐标是Transform加上每个瓦片的位置

	class TileMap:public Level
	{
	public:
		TileSet tileSet;
		std::unordered_map<int, bool> collisionTiles;

	private:
		TileMap(Scene* scene);
		virtual ~TileMap();

	public:

		static TileMap* Create(Scene* scene);

		TileMap& LoadTileSet(const std::string& file);
		TileMap& LoadTileLayer(const std::vector<std::string>& file);
		TileMap& SetCollisionTileID(const std::vector<int>& collisionTileIDs);
		TileMap& SetCollisionTileID(const int tileID);

		TileMap* Apply();

		int GetLayerTileID(const int layerIndex,const int x,const int y) const;
		void SetLayerTileID(const int layerIndex,const int x, const int y,const int tileID);
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
	};

	class Scene final
	{
	//一个场景只有一个level
	//但是一般来说开发者并不知道这件事
	//所以怎么管理Level是一个问题
	private:
		friend class SceneManager;
	private:
		std::string name = "Scene";
		std::vector<Actor*> actors;
		std::vector<Camera*> cameras;

		int mainCameraIndex = -1;

		noa::Level* level = nullptr;

	private:
		Scene(const std::string & name);
		virtual ~Scene();

	public:
		NoaEvent<Scene*> onLoad;
		NoaEvent<Scene*> onStart;
		NoaEvent<Scene*> onUpdate;
		NoaEvent<Scene*> onUnload;

	public:
		Level* GetLevel();
		void SetLevel(Level* map);
		void AddCamera(Camera* camera);
		Camera* GetMainCamera();
		void AddActor(Actor* actor);
		std::string GetName();

		template<class T>
		T* GetLevelAs() {
			return dynamic_cast<T*>(this->level);
		}

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
		static Scene* CreateScene(const std::string & name);

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


