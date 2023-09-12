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

	bool TileMap::IsTile(int code) const
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
		const int collisionTilesCount = collisionTileIDs.size();
		for (int i = 0; i < collisionTilesCount; i++)
		{
			collisionTiles[collisionTileIDs[i]] = true;
		}
	}

	Tile* TileMap::GetTile(int id)
	{
		if (!IsTile(id))
		{
			return nullptr;
		}
		return tileSet[id];
	}

	Scene::Scene(string name)
	{
		this->name = name;
		sceneManager.AddScene(this);
		this->OnEnable();
	}

	Scene::~Scene()
	{
		this->OnDisable();
	}

	Scene * SceneManager::GetActiveScene()
	{
		return activeScene;
	}

	void SceneManager::LoadScene(string sceneName)
	{
		if (ContainKey<string,Scene*>(this->sceneList,sceneName)) 
		{
			activeScene->OnDisable();
			activeScene = sceneList[sceneName];
			activeScene->OnEnable();
			Debug("Load scene:"+sceneName);
		}
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

}