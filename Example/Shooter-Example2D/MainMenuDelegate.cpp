#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvas* canvas = noa::UICanvas::Create(scene);
	
	noa::Button* startButton = noa::Button::Create(canvas);
	startButton->label->text = "START";
	startButton->image->SetSprite(&ResourceManager::defaultImage);

	startButton->transform.position.x = 
		noa::Screen::width / 2 - startButton->transform.scale.x / 2;
	startButton->transform.position.y = 
		noa::Screen::height / 2 - startButton->transform.scale.y/2;

	startButton->clickEvent += []() 
	{
		noa::sceneManager.LoadScene("Game");
	};

}
