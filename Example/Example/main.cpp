#include "NoaEngine.h"
#include "MainMenuUI.h"

void MainMenuDelegateFunc(noa::Scene* scene) {
	MainMenuUI* ui = noa::NObject<MainMenuUI>::Create(scene);
}

class Engine :public noa::NoaEngine {
public:
	Engine() :noa::NoaEngine(
		1920/2,1080/2,noa::WindowMode::WINDOW,noa::GraphicsAPI::OPENGL,"Shooter"
	) 
	{
		noa::Scene* menuScene = noa::Scene::Create("MainMenu");
		menuScene->onLoad += MainMenuDelegateFunc;
		
		noa::SceneManager::LoadScene("MainMenu");
	}

	~Engine() {

	}

};

int main() {
	Engine game;
	game.Run();
	return 0;
}