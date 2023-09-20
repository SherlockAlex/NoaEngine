#include "Scene.h"
#include "NoaEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace noa 
{
	SceneManager sceneManager;

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

	TileMap::TileMap()
	{
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

	void TileMap::SetTileID(const int x, const int y, const int tileID)
	{
		if (x < 0 || x >= w || y < 0 || y >= h)
		{
			return ;
		}
		level[y * w + x] = tileID;
	}

	bool TileMap::IsTile(const int code) const
	{
		return tileSet.count(code) > 0;
	}

	bool TileMap::IsCollisionTile(int tileID) const
	{
		return collisionTiles.count(tileID) > 0;
	}

	bool TileMap::IsCollisionTile(const int x, const int y) const
	{
		if (x < 0 || x >= w || y < 0 || y >= h)
		{
			return true;
		}
		return collisionTiles.count(level[y * w + x]) > 0;
	}

	void TileMap::SetCollisionTileID(std::vector<int> collisionTileIDs)
	{
		//设置Collision Tiles
		const int collisionTilesCount = collisionTileIDs.size();
		for (int i = 0; i < collisionTilesCount; i++)
		{
			collisionTiles[collisionTileIDs[i]] = true;
		}
	}

	Tile* TileMap::GetTile(const int id)
	{
		const bool isTile = tileSet.count(id) > 0;
		if (!isTile)
		{
			return nullptr;
		}
		return tileSet[id];
	}

	Scene::Scene(string name, SceneInfo info)
	{
		this->name = name;
		sceneManager.AddScene(this);
		this->info = info;

	}

	Scene::~Scene()
	{
		
	}

	MapInfo Scene::GetTileMap()
	{
		
		TileMap tileMap = TileMap(
		resource.LoadTileFromTsd(info.mapTileSetPath),
		resource.LoadMapFromCSV(info.mapPath)
		);
		MapFile objectMap = resource.LoadMapFromCSV(info.mapObjectPath);

		MapInfo info = { tileMap,objectMap };

		return info;
	}

	void Scene::AddActor(Actor* actor)
	{
		if (actor == nullptr)
		{
			return;
		}
		this->actors[actor->GetHash()] = actor;
	}

	void Scene::RemoveActor(Actor* actor)
	{
		if (actor == nullptr)
		{
			return;
		}
		this->actors[actor->GetHash()] = nullptr;
		//delete actor;
	}

	void Scene::AddGameObject(GameObjectBuffer gameObject)
	{
		this->gameObjects.push_back(gameObject);
	}

	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		for (auto& object:gameObjects)
		{
			if (object.object == gameObject)
			{
				object.distanceToPlayer = -1;
				object.object = nullptr;
			}
		}
	}

	void Scene::AddRigidbody(Rigidbody* actor)
	{
		if (actor == nullptr)
		{
			return;
		}
		this->rigidbodys[actor->GetHashCode()] = actor;
	}

	void Scene::RemoveRigidbody(Rigidbody* actor)
	{
		if (actor == nullptr)
		{
			return;
		}
		auto it = rigidbodys.find(actor->GetHashCode());
		if (it != rigidbodys.end()) {
			delete it->second; // 释放内存
			rigidbodys.erase(it); // 从map中移除
		}
		//this->rigidbodys[actor->GetHashCode()] = nullptr;
	}

	void Scene::ActorAwake()
	{
		for (auto & e:this->actors)
		{
			Actor* actor = e.second;
			if (actor == nullptr) 
			{
				continue;
			}
			actor->Awake();
		}
	}

	void Scene::ActorOnEnable()
	{
		for (auto& e : this->actors)
		{
			Actor* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->OnEnable();
		}
	}

	void Scene::ActorStart()
	{
		for (auto& e : this->rigidbodys)
		{
			Rigidbody* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->Start();
		}

		for (auto& e : this->actors)
		{
			Actor* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->Start();
		}
	}

	void Scene::ActorUpdate()
	{
		for (auto& e : this->rigidbodys)
		{
			Rigidbody* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->Update();
		}

		for (auto& e : this->actors)
		{
			Actor* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->Update();
		}
	}

	void Scene::ActorOnDisable()
	{
		for (auto& e : this->actors)
		{
			Actor* actor = e.second;
			if (actor == nullptr)
			{
				continue;
			}
			actor->OnDisable();
		}
	}

	Scene * SceneManager::GetActiveScene()
	{
		return activeScene;
	}

	void SceneManager::LoadScene(string sceneName)
	{
		if (!ContainKey<string,Scene*>(this->sceneList,sceneName)) 
		{
			Debug("Load scene:"+ sceneName+"failed");
			return;
		}
		//执行老场景的逐渐卸载
		Destroy();
		//加载新场景
		activeScene = sceneList[sceneName];
		Awake();
		if (activeScene != nullptr)
		{
			activeMapInfo = activeScene->GetTileMap();
		}
		//初始化场景物品
		Start();
		Debug("Load scene:" + sceneName);

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
			activeMapInfo = activeScene->GetTileMap();
		}

		sceneList[scene->name] = scene;

	}

	void SceneManager::Awake()
	{
		//清理actor和rigidbodys
		if (activeScene != nullptr)
		{
			activeScene->Awake();
		}
	}

	void SceneManager::Destroy()
	{
		//清理actor和rigidbodys
		if (activeScene != nullptr)
		{
			activeScene->ActorOnDisable();
			activeScene->Unload();
			activeScene = nullptr;
		}
	}

	void SceneManager::Start()
	{
		if (activeScene != nullptr)
		{
			activeScene->Start();
			activeScene->ActorAwake();
			activeScene->ActorOnEnable();
			activeScene->ActorStart();
		}
		
	}

	void SceneManager::Update()
	{
		if (activeScene == nullptr)
		{
			return;
		}

		activeScene->Update();
		activeScene->ActorUpdate();
		
	}

}