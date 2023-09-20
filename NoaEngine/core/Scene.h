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

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Sprite.h"
#include "Resource.h"
#include <list>

using namespace std;

namespace noa {
	typedef unsigned char Uint8;

	class GameObject;
	class Actor;
	class Rigidbody;

	class LevelMap {
	public:
		vector<int> level;
		vector<vector<int>> levelLayer;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(MapFile & map);
		void Construct(MapFile& map);
		void ConstructLayer(vector<vector<int>> layer);
		
	};

	//��Ƭ��ͼ
	class TileMap:public LevelMap
	{
	public:
		unordered_map<int, Tile*> tileSet;
		unordered_map<int, bool> collisionTiles;
	public:
		/// <summary>
		/// ������Ƭ��ͼ
		/// </summary>
		/// <param name="tileSet">��Ƭ����</param>
		/// <param name="map">��ͼ����</param>
		TileMap();
		TileMap(unordered_map<int,Tile*> tileSet,MapFile map);
		TileMap(unordered_map<int, Tile*> tileSet, vector<MapFile> mapLayer);
		int GetTileID(const int x,const int y) const;
		void SetTileID(const int x, const int y,const int tileID);
		//���������ھ�̬��ȫ��Tile����ʹ���ڴ��������¼�Tile
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x,const int y) const;
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
	};

	struct GameObjectBuffer;
	typedef struct GameObjectBuffer GameObjectBuffer;

	/// <summary>
	/// map,object,tile set
	/// </summary>
	typedef struct SceneInfo
	{
		string mapPath = "";
		string mapObjectPath = "";
		string mapTileSetPath = "";
	}SceneInfo;

	typedef struct MapInfo {
		TileMap mapLayer;
		MapFile objectLayer;
	}MapInfo;

	class Scene 
	{
	public:
		string name = "Scene";
		SceneInfo info;

		unordered_map<size_t, Rigidbody*> rigidbodys;
		unordered_map<size_t, Actor*> actors;//����
		vector<GameObjectBuffer> gameObjects;//��ͼ�õ�
	public:
		Scene(string name, SceneInfo info);
		~Scene();

		MapInfo GetTileMap();

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void Unload() {}

	public:
		void AddActor(Actor* actor);
		void RemoveActor(Actor * actor);

		void AddGameObject(GameObjectBuffer gameObject);
		void RemoveGameObject(GameObject* gameObject);
		
		void AddRigidbody(Rigidbody* actor);
		void RemoveRigidbody(Rigidbody* actor);

		void ActorAwake();
		void ActorOnEnable();
		void ActorStart();
		void ActorUpdate();
		void ActorOnDisable();

		void DestoyScene();

	};

	class SceneManager 
	{
	public:
		//���س���ʱ�������¼�
		MapInfo activeMapInfo;
		Scene * GetActiveScene();
		void LoadScene(string sceneName);
		void AddScene(Scene* scene);

		void Awake();
		void Destroy();
		void Start();
		void Update();

	public:
		//���浱ǰ�����ĸ����actors
		

	private:
		Scene* oldScene = nullptr;
		Scene * activeScene = nullptr;
		unordered_map<string, Scene*> sceneList;
	};

	extern SceneManager sceneManager;

}



#endif // !NOAENGINE_SCENE_H


