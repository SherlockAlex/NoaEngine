#include "RPGGame.h"
#include "GameTestDelegate.h"

RPGGame::RPGGame() 
	:noa::NoaEngine(1920/2,1080/2,noa::WindowMode::WINDOW,noa::GraphicsAPI::OPENGL,"RPG")
{
	noa::Scene* gameTest = noa::sceneManager.CreateScene("Game");
	gameTest->onLoad += GameTestDelegate::OnLoad;

	noa::sceneManager.LoadScene("Game");
}

RPGGame::~RPGGame() {

}

int main() {
	RPGGame game;
	game.Run();
	return 0;
}