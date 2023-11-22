#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Resource.h"
#include "Actor.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "Camera.h"
#include "Debug.h"

using namespace std;

noa::SceneManager noa::sceneManager;

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

int noa::MapLayer::GetTileID(int x, int y) const
{
	if (x < 0 || x >= static_cast<int>(this->w)
		|| y < 0 || y >= static_cast<int>(this->h))
	{
		return -1;
	}
	return this->layer[y * this->w + x];
}

void noa::MapLayer::SetTileID(int x, int y, int value)
{
	if (x < 0 || x >= static_cast<int>(this->w)
		|| y < 0 || y >= static_cast<int>(this->h))
	{
		return;
	}
	this->layer[y * this->w + x] = value;
}

noa::Level::Level(noa::Scene* scene) 
{
	this->scene = scene;
}

noa::Level::~Level()
{
}

void noa::Level::Delete(Level*& level)
{
	delete level;
	level = nullptr;
}

void noa::Level::LoadLayer(const std::vector<std::string>& layerPath)
{
	//读取本地文件
	std::vector<MapLayer> mapLayers;
	for (const auto& file : layerPath)
	{
		MapLayer mapLayer(Resource::LoadMapLayer(file));
		mapLayers.push_back(mapLayer);
	}

	this->Construct(mapLayers);
}

void noa::Level::Construct(const std::vector<MapLayer>& mapLayers)
{
	this->layers = mapLayers;
	//返回地图的最大尺寸
	for (auto& layer : layers)
	{
		if (this->w < layer.w)
		{
			this->w = layer.w;
		}
		if (this->h < layer.h)
		{
			this->h = layer.h;
		}
	}

	if (scene) 
	{
		scene->SetLevel(this);
	}

}

noa::TileMap::TileMap(noa::Scene* scene):Level(scene)
{

}

noa::TileMap::~TileMap()
{

}

noa::TileMap* noa::TileMap::Create(Scene* scene)
{
	noa::TileMap* map = new TileMap(scene);
	return map;
}

noa::TileMap& noa::TileMap::LoadTileSet(const std::string& file) 
{
	this->tileSet = Resource::LoadTileSet(file);
	return *this;
}

noa::TileMap& noa::TileMap::LoadTileLayer(
	const std::vector<std::string>& layerFile
) 
{
	this->LoadLayer(layerFile);
	return *this;
}

noa::TileMap& noa::TileMap::SetCollisionTileID(const std::vector<int>& collisionTileIDs)
{
	//设置Collision Tiles
	const int collisionTilesCount = static_cast<int>(collisionTileIDs.size());
	for (int i = 0; i < collisionTilesCount; i++)
	{
		collisionTiles[collisionTileIDs[i]] = true;
	}
	return *this;
}

noa::TileMap& noa::TileMap::SetCollisionTileID(const int tileID)
{
	this->collisionTiles[tileID] = true;
	return *this;
}

noa::TileMap* noa::TileMap::Apply() {
	return this;
}

int noa::TileMap::GetLayerTileID(const int layerIndex, const int x, const int y) const
{
	if (layerIndex<0||layerIndex>=this->layers.size())
	{
		return -1;
	}
	return layers[layerIndex].GetTileID(x,y);
}

void noa::TileMap::SetLayerTileID(const int layerIndex,const int x, const int y, const int tileID)
{
	if (layerIndex < 0 || layerIndex >= this->layers.size())
	{
		return;
	}
	layers[layerIndex].SetTileID(x,y,tileID);
}

bool noa::TileMap::IsTile(const int code) const
{
	return tileSet.ContainTileID(code);
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
		if (x < 0 || x >= static_cast<int>(layer.w) || y < 0 || y >= static_cast<int>(layer.h))
		{
			continue;
		}
		isCollision = collisionTiles.count(layer.GetTileID(x,y)) > 0;
		if (isCollision)
		{
			return true;
		}
	}
	return isCollision;

}

noa::Tile* noa::TileMap::GetTile(const int id)
{
	return tileSet.GetTileByID(id);
}

noa::Scene::Scene(const std::string& name)
{
	this->name = name;
	actors.reserve(1024 * 10);
}

noa::Scene::~Scene()
{
	DestoyScene();
}

noa::Scene* noa::Scene::CreateChild(const std::string& name) 
{
	noa::Scene* child = new noa::Scene(name);
	this->AddSceneChild(child);
	return child;
}

void noa::Scene::AddSceneChild(noa::Scene* child) 
{
	if (!child) 
	{
		return;
	}
	this->sceneChildren[child->GetName()] = child;
}

void noa::Scene::ActiveSceneChild(const std::string& name) 
{
	if (sceneChildren.count(name)<=0)
	{
		return;
	}
	this->sceneStack.push(sceneChildren.at(name));
}

void noa::Scene::CloseSceneChild() {
	if (this->sceneStack.empty()) 
	{
		return;
	}
	this->sceneStack.pop();
}

noa::Level* noa::Scene::GetLevel()
{
	return this->level;
}

void noa::Scene::SetLevel(Level* map)
{
	PhysicsSystem::SetGrid(map->w, map->h);
	if (this->level)
	{
		this->level->Delete(this->level);
	}
	this->level = map;
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

	if (!sceneStack.empty())
	{
		//子场景Update
		sceneStack.top()->ActorUpdate();
		return;
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		actor->ComponentUpdate();
		actor->Update();
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		actor->LateUpdate();
		actor->ComponentLateUpdate();
	}

	

}

void noa::Scene::DestoyScene()
{
	std::sort(actors.begin(), actors.end());
	auto actorLast = std::unique(actors.begin(), actors.end());
	actors.erase(actorLast, actors.end());

	for (auto& actor : actors)
	{
		if (actor == nullptr)
		{
			continue;
		}
		actor->Delete(actor);
	}

	actors.clear();

	//删除掉level
	this->level->Delete(this->level);

	while (!sceneStack.empty())
	{
		sceneStack.pop();
	}

	if (!sceneChildren.empty())
	{
		for (auto& scene : sceneChildren)
		{
			if (scene.second != nullptr)
			{
				scene.second->onUnload(scene.second);
				scene.second->DestoyScene();
				scene.second->Delete(scene.second);
			}
		}
	}
	sceneChildren.clear();
	this->onUnload(this);
}

void noa::Scene::Delete(noa::Scene*& ptr)
{
	ptr = nullptr;
	delete this;
}

void noa::Scene::SceneChildOnLoad() {
	for (auto& child : sceneChildren)
	{
		if (child.second)
		{
			child.second->onLoad(child.second);
		}
	}

	this->onLoad(this);

}

void noa::Scene::SceneChildOnStart() {
	for (auto& child:sceneChildren) 
	{
		if (child.second)
		{
			child.second->onStart(child.second);
		}
	}

	this->onStart(this);

}

void noa::Scene::SceneChildRender() {

	for (auto& child : sceneChildren)
	{
		for (const auto& actor : child.second->actors)
		{
			if (actor == nullptr 
				|| !actor->GetActive() 
				|| actor->isRemoved)
			{
				continue;
			}
			actor->Render();
			actor->ComponentRender();
		}
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr
			|| !actor->GetActive()
			|| actor->isRemoved)
		{
			continue;
		}
		actor->Render();
		actor->ComponentRender();
	}

}

void noa::Scene::SceneChildOnUpdate() 
{

	if (sceneStack.empty()) 
	{
		onUpdate.Invoke(this);
		return;
	}
	noa::Scene* scene = sceneStack.top();
	scene->onUpdate(scene);

}

void noa::Scene::SceneChildOnTick() {

	for (auto& child:sceneChildren) 
	{
		if (child.second) 
		{
			child.second->SceneChildOnTick();
		}
		
	}

	onTick.Invoke(this);

}

noa::Actor* noa::Scene::FindActorWithTag(const string& tag)
{
	noa::Actor* buffer = nullptr;
	for (auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		if (actor->tag == tag)
		{
			buffer = actor;
			break;
		}
	}

	if (buffer)
	{
		return buffer;
	}

	for (auto& child:sceneChildren) 
	{
		if (child.second) 
		{
			buffer = child.second->FindActorWithTag(tag);
		}
	}

	return buffer;
}

std::vector<noa::Actor*> noa::Scene::FindActorsWithTag(const std::string& tag)
{
	std::vector<Actor*> target;

	for (auto& actor : this->actors)
	{
		if (actor == nullptr 
			|| actor->isRemoved 
			|| !actor->GetActive() 
			|| actor->tag != tag)
		{
			continue;
		}
		target.push_back(actor);
	}

	for (auto& child:sceneChildren) 
	{
		if (!child.second)
		{
			continue;
		}

		std::vector<Actor*> childTarget = 
			child.second->FindActorsWithTag(tag);

		for (auto& actor:childTarget) 
		{
			target.push_back(actor);
		}

	}

	return target;
}

std::string noa::Scene::GetName()
{
	return this->name;
}

noa::Scene* noa::SceneManager::CreateScene(const std::string& name)
{
	noa::Scene* scene = new Scene(name);
	sceneManager.AddScene(scene);
	return scene;
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
		activeScene->SceneChildOnLoad();
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
		activeScene->SceneChildOnStart();
	}
}

void noa::SceneManager::Update()
{
	if (activeScene == nullptr)
	{
		return;
	}

	activeScene->ActorUpdate();
	activeScene->SceneChildRender();

	//更新物理系统
	PhysicsSystem::Update(PhysicsSystem::step);

	activeScene->SceneChildOnUpdate();
	activeScene->SceneChildOnTick();

	if (!done && oldScene != nullptr && oldScene != activeScene)
	{
		oldScene->DestoyScene();
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
				scene.second->Delete(scene.second);
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
	
	return activeScene->FindActorWithTag(tag);
	
}

std::vector<noa::Actor*> noa::SceneManager::FindActorsWithTag(const std::string& tag)
{
	if (this->activeScene == nullptr)
	{
		return std::vector<Actor*>();
	}
	return this->activeScene->FindActorsWithTag(tag);
}
