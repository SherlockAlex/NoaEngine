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
#include "Core.h"

namespace noa {

	class NOA_API Actor;
	class NOA_API Rigidbody;
	class NOA_API ScriptableActor;
	class NOA_API Camera;

	class NOA_API Scene;

	class NOA_API Scene final
	{
	private:
		friend class SceneManager;
	private:
		Scene(const std::string& name);
		virtual ~Scene();

	public:

		static Scene* Create(const std::string& name);

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

	private:
		std::string name = "Scene";
		std::vector<Actor*> actors;
		std::map<std::string, Scene*> sceneChildren;
		std::stack<noa::Scene*> sceneStack;
	public:
		NoaEvent<Scene*> onLoad;
		NoaEvent<Scene*> onStart;
		NoaEvent<Scene*> onUpdate;
		NoaEvent<Scene*> onTick;
		NoaEvent<Scene*> onUnload;

	};

}



#endif // !NOAENGINE_SCENE_H


