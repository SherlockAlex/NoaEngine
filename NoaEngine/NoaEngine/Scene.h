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

namespace noa {


	typedef unsigned char Uint8;

	class Actor;
	class Rigidbody;
	class ScriptableActor;
	class Camera;

	//地图的层级
	class MapLayer {
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
	class Level {
	public:
		std::vector<MapLayer> layers;
		int w = 0;
		int h = 0;
	protected:
		Level(const std::vector<MapLayer> & layer);
		Level(const std::vector<std::string> & layerPath);
		virtual ~Level();

	public:
		void Delete(Level *& level);
		
		template<class T>
		T* GetLevelAs() {
			return dynamic_cast<T*>(this);
		}

		void Construct(const std::vector<MapLayer>& layer);

	};

	//一张合格的tilemap
	//tile set
	//collisionTiles
	//
	//mapLayer
	//objectLayer

	//TileMap的耦合程度过高
	//Camera
	//Rigidbody
	//Collider
	//Scene
	
	class Scene;
	//瓦片地图
	class TileMap:public Level
	{
	public:
		std::unordered_map<int, Tile> tileSet;
		std::unordered_map<int, bool> collisionTiles;
	

	private:

		TileMap(const std::unordered_map<int,Tile> & tileSet
			,const std::vector<MapLayer> & mapLayer);

		TileMap(const std::string& tileSetFile
			, const std::vector<std::string>& layerFile);

		virtual ~TileMap();

	public:

		static TileMap* Create(const std::string& tileSetFile
			, const std::vector<std::string>& layerFile
			, Scene * scene
		);

		int GetLayerTileID(const int layerIndex,const int x,const int y) const;
		void SetLayerTileID(const int layerIndex,const int x, const int y,const int tileID);
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(const std::vector<int> & collisionTileIDs);
	};


	/*typedef struct MapInfo {
		std::shared_ptr<TileMap> mapLayer;
		std::shared_ptr<MapFile> objectLayer;

	}MapInfo;*/

	class Scene final
	{

	private:
		friend class SceneManager;
	private:
		std::string name = "Scene";
		std::vector<Actor*> actors;
		std::vector<Camera*> cameras;

		int mainCamera = -1;

		noa::Level* tileMap = nullptr;

	private:
		Scene(const std::string & name);
		virtual ~Scene();

	public:
		NoaEvent<Scene*> onLoad;
		NoaEvent<Scene*> onStart;
		NoaEvent<Scene*> onUpdate;
		NoaEvent<Scene*> onUnload;

	public:
		Level* GetTileMap();
		void SetTileMap(Level* map);
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


