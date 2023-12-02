#ifndef NOAENGINE_SCENEMANAGER_H
#define NOAENGINE_SCENEMANAGER_H

#include <vector>
#include <map>
#include <string>

#include "Scene.h"
#include "Core.h"

namespace noa {
	class NOA_API Scene;
	class NOA_API Actor;

	class NOA_API SceneManager
	{
	public:

		static Scene* GetActiveScene();
		static void LoadScene(const std::string& sceneName);

		static void DestroySceneManager();

		static void Quit();

		template<class T>
		static std::vector<T*> FindActorsWithType() {
			std::vector<T*> results;
			if (activeScene == nullptr)
			{
				return results;
			}
			results = activeScene->FindActorsWithType<T>();
			return results;

		}

		template<class T>
		static T* FindActorWithType() {
			if (activeScene == nullptr)
			{
				return nullptr;
			}
			return activeScene->FindActorWithType<T>();
		}

		static Actor* FindActorWithTag(const std::string& tag);

		static std::vector<Actor*> FindActorsWithTag(const std::string& tag);

	private:
		static void Awake();
		static void Destroy();
		static void Start();
		static void Update();

		static void AddScene(Scene* scene);

	private:
		static Scene* oldScene;
		static Scene* activeScene;
		static Scene* nextScene;
		static std::map<std::string, Scene*> sceneList;

		static bool done;
		static bool isLoading;

	public:
		static bool isQuit;
		static bool quited;

	private:
		friend class NoaEngine;
		friend class Scene;

	};
}

#endif // !NOAENGINE_SCENEMANAGER_H
