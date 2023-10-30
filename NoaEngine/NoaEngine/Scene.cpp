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

noa::MapLayer::MapLayer(const LayerFile& layer)
{
	//层级
	this->layer = layer.image;
	this->w = layer.w;
	this->h = layer.h;
}

noa::MapLayer::~MapLayer()
{

}

int noa::MapLayer::Find(int x, int y) const
{
	if (x<0||x>=this->w
		||y<0||y>=this->h) 
	{
		return -1;
	}
	return this->layer[y * this->w + x];
}

void noa::MapLayer::Set(int x, int y, int value)
{
	if (x < 0 || x >= this->w
		|| y < 0 || y >= this->h)
	{
		return;
	}
	this->layer[y * this->w + x] = value;
}

noa::LevelMap::LevelMap(const std::vector<noa::MapLayer>& mapLayers)
{
	this->layers = mapLayers;
	//返回地图的最大尺寸
	for (auto & layer:layers) 
	{
		if (this->w<layer.w)
		{
			this->w = layer.w;
		}
		if (this->h < layer.h)
		{
			this->h = layer.h;
		}
	}
}

noa::TileMap::TileMap(
	const std::unordered_map<int, Tile>& tileSet
	, const std::vector<noa::MapLayer>& mapLayer
):noa::LevelMap(mapLayer)
{
	this->tileSet = tileSet;
}

noa::TileMap::~TileMap()
{

}

int noa::TileMap::GetLayerTileID(const int layerIndex, const int x, const int y) const
{
	if (layerIndex<0||layerIndex>=this->layers.size())
	{
		return -1;
	}
	return layers[layerIndex].Find(x,y);
}

void noa::TileMap::SetLayerTileID(const int layerIndex,const int x, const int y, const int tileID)
{
	if (layerIndex < 0 || layerIndex >= this->layers.size())
	{
		return;
	}
	layers[layerIndex].Set(x,y,tileID);
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
	

	//会去遍历所有的layer
	//只要有一个layer是，那么就是
	bool isCollision = false;
	for (const auto & layer:this->layers)
	{
		if (x < 0 || x >= layer.w || y < 0 || y >= layer.h)
		{
			continue;
		}
		isCollision = collisionTiles.count(layer.Find(x,y)) > 0;
		if (isCollision)
		{
			return true;
		}
	}
	return isCollision;

}

void noa::TileMap::SetCollisionTileID(const std::vector<int>& collisionTileIDs)
{
	//设置Collision Tiles
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

noa::TileMap * noa::Scene::GetTileMap()
{
	return this->tileMap;
}

void noa::Scene::SetTileMap(TileMap* map)
{
	PhysicsSystem::SetGrid(map->w, map->h);
	this->tileMap = map;
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

	for (const auto& actor : actors)
	{
		if (actor == nullptr || !actor->GetActive() || actor->isRemoved)
		{
			continue;
		}
		actor->ComponentLateUpdate();
		actor->LateUpdate();
	}

	//更新物理系统
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
		//删除调Camera
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

noa::Scene* noa::SceneManager::CreateScene(const std::string& name)
{
	return new Scene(name);
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
		activeScene->onLoad.Invoke(activeScene);
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
		activeScene->onStart.Invoke(activeScene);
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
	activeScene->onUpdate.Invoke(activeScene);

	if (!done && oldScene != nullptr && oldScene != activeScene)
	{
		oldScene->DestoyScene();
		oldScene->onUnload.Invoke(activeScene);
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
