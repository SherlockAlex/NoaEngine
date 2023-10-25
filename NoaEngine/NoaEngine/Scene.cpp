#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Actor.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "Camera.h"
#include "Debug.h"

using namespace std;

namespace noa 
{

	SceneManager sceneManager;

}

noa::LevelMap::LevelMap() {

}

noa::LevelMap::LevelMap(const MapFile& map)
{
	this->w = map.w;
	this->h = map.h;

	level = map.image;

}

void noa::LevelMap::Construct(const MapFile& map)
{
	this->w = map.w;
	this->h = map.h;

	level = map.image;

}

void noa::LevelMap::ConstructLayer(const std::vector<std::vector<int>>& layer)
{
	levelLayer = layer;
}

noa::TileMap::TileMap()
{
}

noa::TileMap::TileMap(const std::unordered_map<int, Tile>& tileSet, const MapFile& map) :LevelMap(map)
{
	this->tileSet = tileSet;
}

noa::TileMap::TileMap(const std::unordered_map<int, Tile>& tileSet, const std::vector<MapFile>& mapLayer)
{
	this->tileSet = tileSet;
	MapFile map;
	map.image = mapLayer[0].image;
	map.w = mapLayer[0].w;
	map.h = mapLayer[0].h;

	std::vector<vector<int>> layer;

	for (int i = 0; i < mapLayer.size(); i++)
	{
		layer.push_back(mapLayer[i].image);
	}

	for (int i = 1; i < mapLayer.size(); i++)
	{
		for (int j = 0; j < mapLayer[i].image.size(); j++)
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

noa::TileMap::~TileMap()
{

}

int noa::TileMap::GetTileID(const int x, const int y) const
{
	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return -1;
	}
	return level[y * w + x];
}

void noa::TileMap::SetTileID(const int x, const int y, const int tileID)
{
	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return;
	}
	level[y * w + x] = tileID;
}

bool noa::TileMap::IsTile(const int code) const
{
	return tileSet.count(code) > 0;
}

bool noa::TileMap::IsCollisionTile(int tileID) const
{
	return collisionTiles.count(tileID) > 0;
}

bool noa::TileMap::IsCollisionTile(const int x, const int y) const
{
	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return true;
	}
	return collisionTiles.count(level[y * w + x]) > 0;
}

void noa::TileMap::SetCollisionTileID(const std::vector<int>& collisionTileIDs)
{
	//…Ë÷√Collision Tiles
	const int collisionTilesCount = static_cast<int>(collisionTileIDs.size());
	for (int i = 0; i < collisionTilesCount; i++)
	{
		collisionTiles[collisionTileIDs[i]] = true;
	}
}

noa::Tile* noa::TileMap::GetTile(const int id)
{
	const bool isTile = tileSet.count(id) > 0;
	if (!isTile)
	{
		return nullptr;
	}
	return &tileSet[id];
}

noa::Scene::Scene(const std::string& name)
{
	this->name = name;
	sceneManager.AddScene(this);
	actors.reserve(1024 * 10);
}

noa::Scene::~Scene()
{
	DestoyScene();
}

void noa::Scene::SetTileMap(TileMap* map)
{
	PhysicsSystem::SetGrid(map->w, map->h);
}

void noa::Scene::AddCamera(Camera* camera)
{
	if (camera == nullptr)
	{
		return;
	}
	this->cameras.push_back(camera);
	if (mainCamera == -1)
	{
		mainCamera = 0;
	}
}

noa::Camera* noa::Scene::GetMainCamera()
{
	if (mainCamera < 0 || mainCamera >= cameras.size())
	{
		return nullptr;
	}
	return cameras[mainCamera];
}

void noa::Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
	{
		return;
	}

	actors.push_back(actor);

}

void noa::Scene::ActorUpdate()
{

	for (const auto& actor : actors)
	{
		if (actor == nullptr || !actor->GetActive() || actor->isRemoved)
		{
			continue;
		}
		actor->ComponentUpdate();
		actor->Update();
	}

	PhysicsSystem::Update(3);

}

void noa::Scene::DestoyScene()
{
	std::sort(actors.begin(), actors.end());
	auto actorLast = std::unique(actors.begin(), actors.end());
	actors.erase(actorLast, actors.end());

	std::sort(cameras.begin(), cameras.end());
	auto cameraLast = std::unique(cameras.begin(), cameras.end());
	cameras.erase(cameraLast, cameras.end());

	for (auto& actor : actors)
	{
		if (actor == nullptr)
		{
			continue;
		}
		actor->Delete(actor);
	}

	for (auto& camera : cameras)
	{
		if (camera == nullptr)
		{
			continue;
		}
		//…æ≥˝µ˜Camera
		camera->Delete(camera);

	}
	mainCamera = -1;

	actors.clear();
	cameras.clear();
}

void noa::Scene::Delete()
{
	delete this;
}

noa::Actor* noa::Scene::FindActorWithTag(const string& tag)
{
	noa::Actor* buffer = nullptr;
	for (auto& actor : actors)
	{
		if (actor == nullptr || !actor->GetActive() || actor->isRemoved)
		{
			continue;
		}
		if (actor->tag == tag)
		{
			buffer = actor;
			break;
		}
	}
	return buffer;
}

std::vector<noa::Actor*> noa::Scene::FindActorsWithTag(const std::string& tag)
{
	std::vector<Actor*> target;

	for (auto& actor : this->actors)
	{
		if (actor == nullptr || actor->isRemoved || !actor->GetActive() || actor->tag != tag)
		{
			continue;
		}
		target.push_back(actor);
	}

	return target;
}

std::string noa::Scene::GetName()
{
	return this->name;
}

void noa::Scene::ApplyCamera()
{
	if (mainCamera < 0 || mainCamera >= cameras.size())
	{
		return;
	}
	cameras[mainCamera]->Render();
}

noa::Scene* noa::SceneManager::GetActiveScene()
{
	return activeScene;
}

void noa::SceneManager::LoadScene(const std::string& sceneName)
{

	if (isLoading)
	{
		return;
	}

	if (this->sceneList.count(sceneName) <= 0)
	{
		Debug::Error("Load scene:" + sceneName + "failed");
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

void noa::SceneManager::AddScene(Scene* scene)
{
	if (scene == nullptr)
	{
		return;
	}
	if (activeScene == nullptr)
	{
		activeScene = scene;
	}

	sceneList[scene->name] = scene;

}

void noa::SceneManager::Awake()
{
	if (activeScene != nullptr)
	{
		activeScene->Awake();
	}
}

void noa::SceneManager::Destroy()
{
	if (activeScene != nullptr)
	{
		oldScene = activeScene;
		activeScene = nullptr;
	}
}

void noa::SceneManager::Start()
{
	if (activeScene != nullptr)
	{
		activeScene->Start();
	}

}

void noa::SceneManager::Update()
{
	if (activeScene == nullptr)
	{
		return;
	}

	activeScene->ActorUpdate();
	activeScene->ApplyCamera();
	activeScene->Update();

	if (!done && oldScene != nullptr && oldScene != activeScene)
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

noa::SceneManager::~SceneManager()
{
	if (!sceneList.empty())
	{
		for (auto& scene : sceneList)
		{
			if (scene.second == nullptr)
			{
				scene.second->Delete();
			}
		}
	}
	sceneList.clear();
}

void noa::SceneManager::Quit()
{
	isQuit = true;

}

noa::Actor* noa::SceneManager::FindActorWithTag(const std::string& tag)
{
	if (this->activeScene == nullptr)
	{
		return nullptr;
	}
	return this->activeScene->FindActorWithTag(tag);
}

std::vector<noa::Actor*> noa::SceneManager::FindActorsWithTag(const std::string& tag)
{
	if (this->activeScene == nullptr)
	{
		return std::vector<Actor*>();
	}
	return this->activeScene->FindActorsWithTag(tag);
}
