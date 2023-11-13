#pragma once
namespace noa {

	class Scene;

	class EditorSceneDelegate
	{
	public:
		static void OnLoad(Scene* scene);
		static void OnUpdate(Scene* scene);
	};
}
