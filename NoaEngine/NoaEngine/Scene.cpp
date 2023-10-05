#include "Scene.h"
#include "NoaEngine.h"
#include "PhysicsSystem.h"

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

	}

	void LevelMap::Construct(MapFile& map)
	{
		this->w = map.w;
		this->h = map.h;

		level = map.image;

	}

	void LevelMap::ConstructLayer(std::vector<std::vector<int>> layer)
	{
		levelLayer = layer;
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
		//…Ë÷√Collision Tiles
		const int collisionTilesCount = static_cast<int>(collisionTileIDs.size());
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
		destroyActors.reserve(4096);
	}

	Scene::~Scene()
	{
		DestoyScene();
	}

	void Scene::SetTileMap(TileMap* map)
	{
		PhysicsSystem::SetGrid(map->w, map->h);
	}

	void Scene::AddActor(Actor* actor)
	{
		if (actor == nullptr)
		{
			return;
		}

		this->actors[actor->GetHash()] = actor;
		destroyActors.push_back(actor);

	}

	void Scene::RemoveActor(Actor* actor)
	{

		if (actor == nullptr)
		{
			return;
		}
		actor->SetActive(false);

		actor->ComponentOnDestroy();
		actor->OnDestroy();

		actors[actor->GetHash()] = nullptr;

	}

	void Scene::ActorUpdate()
	{
		const auto actorLast = actors.end();
		for (auto i = actors.begin(); i != actorLast; i++)
		{
			if (i->second == nullptr || (!i->second->GetActive()))
			{
				continue;
			}
			i->second->ComponentUpdate();
		}

		PhysicsSystem::Update(3);

		for (auto i = actors.begin(); i != actorLast; i++)
		{
			if (i->second == nullptr || (!i->second->GetActive()))
			{
				continue;
			}
			i->second->Update();
		}

	}

	void Scene::DestoyScene()
	{
		std::sort(destroyActors.begin(), destroyActors.end());
		auto actorLast = std::unique(destroyActors.begin(), destroyActors.end());
		destroyActors.erase(actorLast, destroyActors.end());

		for (int i = 0; i < destroyActors.size(); i++)
		{
			if (destroyActors[i] == nullptr)
			{
				continue;
			}
			destroyActors[i]->Delete();
		}

		actors.clear();
		destroyActors.clear();

	}

	Scene * SceneManager::GetActiveScene()
	{
		return activeScene;
	}

	void SceneManager::LoadScene(string sceneName)
	{
		if (isLoading) 
		{
			return;
		}
		if (!ContainKey<string,Scene*>(this->sceneList,sceneName)) 
		{
			Debug::Error("Load scene:"+ sceneName+"failed");
			return;
		}
		nextScene = sceneList[sceneName];
		done = false;
		isLoading = true;

		oldScene = activeScene;
		activeScene = nextScene;
		nextScene = nullptr;
		
		Awake();
		Start();

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
		}

		sceneList[scene->name] = scene;

	}

	void SceneManager::Awake()
	{
		if (activeScene != nullptr)
		{
			activeScene->Awake();
		}
	}

	void SceneManager::Destroy()
	{
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
		}
		
	}

	void SceneManager::Update()
	{
		if (activeScene == nullptr)
		{
			return;
		}
		

		activeScene->Update();
		spriteRendererInstances.clear();
		activeScene->ActorUpdate();

		if (!done&&oldScene != nullptr && oldScene != activeScene)
		{
			oldScene->DestoyScene();
			oldScene->Unload();
			oldScene = nullptr;
		}

		if (!done)
		{
			done = true;
			isLoading = false;	
		}

		


	}

	SceneManager::~SceneManager()
	{
		if (!sceneList.empty()) 
		{
			for (auto i = sceneList.begin();i!=sceneList.end();i++)
			{
				if (i->second == nullptr) 
				{
					continue;
				}
				i->second->Delete();
			}
		}
		sceneList.clear();
	}

	void SceneManager::Quit()
	{
		isQuit = true;
		
	}

}