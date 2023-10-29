#include "MainMenuLevel.h"
#include "WolfResource.h"

static UICanvas* canvas = nullptr;
static Image* background = nullptr;
static Button* startButton = nullptr;


void MainMenuDelegate::OnLoad(noa::Scene * scene)
{

	canvas = UICanvas::Create(scene);
	background = Image::Create(canvas);
	startButton = Button::Create(canvas);

	background->SetSprite(& wolfResource->backgroundSprite);
	background->transform.scale = { Screen::width,Screen::height };
	background->transform.position = { 0,0 };

	startButton->image->color = WHITE;
	startButton->transform.scale = { (int)(0.2f * Screen::width),(int)(0.2f* Screen::height) };
	startButton->transform.position.x = 0.5 * Screen::width - 0.5 * startButton->transform.scale.x;
	startButton->transform.position.y = 0.5 * Screen::height;
	startButton->label->color = BLACK;
	startButton->label->text = "START";
	startButton->AddClickEvent([scene]()
		{
			sceneManager.LoadScene("SecondFloor");
		}
	);



}

