#pragma once

union SDL_Event;
namespace noa {

	class EditorGUI
	{
	public:
		static void Start();
		static void BeforeUpdate();
		static void Update();
		static void Render();
		static void OnExit();
	private:
		static SDL_Event* ioEvent;
	};

}

