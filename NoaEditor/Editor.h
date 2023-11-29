#pragma once

#include <vector>

union SDL_Event;
namespace noa {

	class EditorWindow;

	class Editor
	{
	public:
		static void Start();
		static void BeforeUpdate();
		static void Update();
		static void Render();
		static void OnExit();

		static void AddEditorWindow(noa::EditorWindow* window);

	private:
		static SDL_Event* ioEvent;
		static std::vector<noa::EditorWindow*> windows;
	};

}

