#include "Shooter.h"
#include "MainMenu.h"



Shooter::Shooter()
	:noa::NoaEngine(1920/2,1080/2,noa::WindowMode::WINDOW,"Shooter-2D")
{
	noa::Scene* mainMenu = noa::sceneManager.CreateScene("MainMenu");
	mainMenu->onLoad += [this](noa::Scene * scene) 
	{
			//定义场景内的Actor
			noa::UICanvas* canvas = noa::UICanvas::Create(scene);

			noa::Button* startButton = noa::Button::Create(canvas);
			startButton->transform.position.x = noa::Screen::width / 2;
			startButton->transform.position.y = noa::Screen::height / 2;
			startButton->text->text = "StartGame";
			startButton->text->color = noa::WHITE;
			startButton->text->size = 50;
	};

	noa::sceneManager.LoadScene("MainMenu");

}
