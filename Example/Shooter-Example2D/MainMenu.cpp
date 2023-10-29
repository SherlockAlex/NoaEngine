#include "MainMenu.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvas* canvas = noa::UICanvas::Create(scene);
	
	noa::Button* startButton = noa::Button::Create(canvas);
	startButton->label->text = "START";
}
