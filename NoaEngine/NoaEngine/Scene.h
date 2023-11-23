#ifndef NOAENGINE_SCENE_H
#define NOAENGINE_SCENE_H

#include <stack>
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

#include "Debug.h"
#include "NObject.h"

namespace noa {

	class Actor;
	class Rigidbody;
	class ScriptableActor;
	class Camera;

	

	//关卡地图
	//一张地图是由许多的地图层组成的

	class Scene;

	class Scene final
	{
	private:
		friend class SceneManager;
	private:
		std::string name = "Scene";
		std::vector<Actor*> actors;

		std::map<std::string, Scene*> sceneChildren;
		std::stack<noa::Scene*> sceneStack;

	private:
		Scene(const std::string& name);
		virtual ~Scene();
	public:
		NoaEvent<Scene*> onLoad;
		NoaEvent<Scene*> onStart;
		NoaEvent<Scene*> onUpdate;
		NoaEvent<Scene*> onTick;
		NoaEvent<Scene*> onUnload;

	public:

		Scene* CreateChild(const std::string& name);
		void ActiveSceneChild(const std::string& name);
		void CloseSceneChild();

		void AddActor(Actor* actor);
		std::string GetName();

	private:
		void AddSceneChild(noa::Scene* scene);

		Actor* FindActorWithTag(const std::string& tag);
		std::vector<Actor*> FindActorsWithTag(const std::string& tag);

		template<class T>
		std::vector<T*> FindActorsWithType() {
			std::vector<T*> results;
			for (auto& actor : this->actors)
			{
				if (actor == nullptr
					|| !actor->GetActive()
					|| actor->isRemoved
					)
				{
					continue;
				}
				T* buffer = dynamic_cast<T*>(actor);
				if (buffer != nullptr)
				{
					results.push_back(buffer);
				}

				for (auto& child : sceneChildren)
				{
					std::vector<T*> childResults = child.second->FindActorsWithType<T>();
					for (auto& childResult : childResults)
					{
						results.push_back(childResult);
					}
				}

			}
			return results;
		}

		template<class T>
		T* FindActorWithType() {
			T* buffer = nullptr;

			for (auto& actor : this->actors)
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

			if (buffer != nullptr)
			{
				return buffer;
			}

			for (auto& child : sceneChildren)
			{
				buffer = child.second->FindActorWithType<T>();
				if (buffer != nullptr)
				{
					break;
				}
			}

			return buffer;

		}

	private:
		void ActorUpdate();
		void DestoyScene();

		void Delete(noa::Scene*& ptr);

		void SceneChildOnLoad();
		void SceneChildOnStart();
		void SceneChildRender();
		void SceneChildOnUpdate();
		void SceneChildOnTick();

	};


	class SceneManager
	{

	private:
		friend class NoaEngine;
		friend class Scene;

	public:
		static Scene* CreateScene(const std::string& name);

		Scene* GetActiveScene();
		void LoadScene(const std::string& sceneName);

		~SceneManager();

		void Quit();

		template<class T>
		std::vector<T*> FindActorsWithType() {
			std::vector<T*> results;
			if (this->activeScene == nullptr)
			{
				return results;
			}
			results = this->activeScene->FindActorsWithType<T>();
			return results;

		}

		template<class T>
		T* FindActorWithType() {
			if (this->activeScene == nullptr)
			{
				return nullptr;
			}
			return activeScene->FindActorWithType<T>();
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
		Scene* activeScene = nullptr;
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


