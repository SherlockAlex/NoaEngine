#include "Scene.h"
#include "NoaEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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

	void LevelMap::ConstructLayer(std::vector<std::vector<int>> layer)
	{
		levelLayer = layer;
		Debug("Update the layer map");
	}

	TileMap::TileMap()
	{
	}

	TileMap::TileMap(unordered_map<int, Tile> tileSet, MapFile map) :LevelMap(map)
	{
		this->tileSet = tileSet;
	}

	TileMap::TileMap(unordered_map<int, Tile> tileSet, std::vector<MapFile> mapLayer)
	{
		this->tileSet = tileSet;
		MapFile map;
		map.image = mapLayer[0].image;
		map.w = mapLayer[0].w;
		map.h = mapLayer[0].h;

		std::vector<vector<int>> layer;

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

	TileMap::~TileMap()
	{

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
		return &tileSet[id];
	}

	Scene::Scene(string name)
	{
		this->name = name;
		sceneManager.AddScene(this);
		//this->info = info;

	}

	Scene::~Scene()
	{
		//清楚所有的资源
		DestoyScene();
		
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

		actors[actor->GetHash()] = nullptr;
		destroyActors.push_back(actor);
		/*auto it = find(destroyActors.begin(),destroyActors.end(),actor);
		if (it == destroyActors.end()) 
		{
			destroyActors.push_back(actor);
		}*/

	}

	void Scene::AddGameObject(GameObjectBuffer gameObject)
	{
		this->gameObjects.push_back(gameObject);
	}

	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr)
		{
			return;
		}

		// 使用迭代器查找要删除的对象
		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
			if (it->object == gameObject) 
			{
				gameObjects.erase(it); // 删除匹配的对象
				break; // 一旦删除，就可以退出循环
			}
		}

		
	}

	void Scene::AddRigidbody(Rigidbody* rigid)
	{
		if (rigid == nullptr)
		{
			return;
		}
		this->rigidbodys[rigid->GetHashCode()] = rigid;
	}

	void Scene::RemoveRigidbody(Rigidbody* rigid)
	{
		if (rigid == nullptr)
		{
			return;
		}
		rigidbodys[rigid->GetHashCode()] = nullptr;
		destroyRigids.push_back(rigid);
		/*auto it = find(destroyRigids.begin(),destroyRigids.end(),rigid);
		if (it == destroyRigids.end())
		{
			destroyRigids.push_back(rigid);
		}*/
	}

	void Scene::ActorAwake()
	{
		for (auto e:this->actors)
		{
			if (e.second == nullptr)
			{
				continue;
			}
			Actor* actor = e.second;
			actor->Awake();
		}
	}

	void Scene::ActorOnEnable()
	{
		for (auto e : this->actors)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Actor* actor = e.second;
			actor->OnEnable();
		}
	}

	void Scene::ActorStart()
	{
		for (auto e : this->rigidbodys)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Rigidbody* actor = e.second;
			actor->Start();
		}

		for (auto e : this->actors)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Actor* actor = e.second;
			actor->Start();
		}
	}

	void Scene::ActorUpdate()
	{
		/*for (auto & e : this->rigidbodys)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Rigidbody* rigid = e.second;
			rigid->Update();
		}

		for (auto & e : this->actors)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Actor* actor = e.second;
			actor->Update();
		}*/
		
		for (auto i = rigidbodys.begin();i!=rigidbodys.end();i++) 
		{
			if (i->second == nullptr)
			{
				continue;
			}
			Rigidbody * rigid = i->second;
			rigid->Update();
		}

		for (auto i = actors.begin();i!=actors.end();i++) 
		{
			if (i->second == nullptr) 
			{
				continue;
			}
			Actor * actor = i->second;
			actor->Update();
		}

	}

	void Scene::ActorOnDisable()
	{
		for (auto & e : this->actors)
		{
			
			if (e.second == nullptr)
			{
				continue;
			}
			Actor* actor = e.second;
			actor->OnDisable();
		}
	}

	void Scene::DestoyScene()
	{

		while (!destroyRigids.empty()||!destroyActors.empty())
		{

			for (auto & e:actors) 
			{
				if (e.second == nullptr)
				{
					continue;
				}
				auto it = find(destroyActors.begin(), destroyActors.end(), e.second);
				if (it == destroyActors.end())
				{
					destroyActors.push_back(e.second);
				}
				
			}

			for (auto & e : rigidbodys)
			{
				if (e.second == nullptr)
				{
					continue;
				}
				
				auto it = find(destroyRigids.begin(), destroyRigids.end(), e.second);
				if (it == destroyRigids.end())
				{
					destroyRigids.push_back(e.second);
				}
			}

			auto rigidLast = std::unique(destroyRigids.begin(), destroyRigids.end());
			destroyRigids.erase(rigidLast, destroyRigids.end());

			auto actorLast = std::unique(destroyActors.begin(), destroyActors.end());
			destroyActors.erase(actorLast, destroyActors.end());

			for (int i = 0;i<destroyRigids.size();i++)
			{
				//会出现野指针的情况，就是一个值会出现两次
				if (destroyRigids[i] == nullptr)
				{
					continue;
				}
				delete destroyRigids[i];
			}

			for (int i = 0; i < destroyActors.size(); i++)
			{
				if (destroyActors[i] == nullptr)
				{
					continue;
				}
				destroyActors[i]->OnDestroy();
				destroyActors[i]->Delete();
			}

			actors.clear();
			gameObjects.clear();
			rigidbodys.clear();
			destroyActors.clear();
			destroyRigids.clear();

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
		nextScene = sceneList[sceneName];
		done = false;


		//执行老场景的逐渐卸载
		//Destroy();
		//加载新场景
		//等带所有的update执行完后
		oldScene = activeScene;
		activeScene = nextScene;
		nextScene = nullptr;

		

		Awake();
		/*if (activeScene != nullptr)
		{
			activeMapInfo = activeScene->GetTileMap();
		}*/
		//初始化场景物品
		Start();

		

		//done = true;

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
			//activeMapInfo = activeScene->GetTileMap();
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
			oldScene = activeScene;
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

		if (!done)
		{
			//处理老场景资源
			if (oldScene != nullptr&&oldScene!=activeScene)
			{
				oldScene->ActorOnDisable();
				oldScene->Unload();
				oldScene->DestoyScene();
				oldScene = nullptr;
				done = true;
			}
		}

	}

	SceneManager::~SceneManager()
	{
		
	}

}