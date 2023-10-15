#include "MainMenuLevel.h"
#include "WolfResource.h"

MainMenuLevel::MainMenuLevel():Scene("MainMenu")
{

}

MainMenuLevel::~MainMenuLevel()
{
	
}

MainMenuLevel* MainMenuLevel::Create()
{
	return new MainMenuLevel;
}

void MainMenuLevel::Awake()
{

	canvas = UICanvas::Create(this);
	background = Image::Create(canvas);
	startButton = Button::Create(canvas);

	background->SetSprite(& wolfResource->backgroundSprite);
	background->transform.scale = { pixelWidth,pixelHeight };
	background->transform.position = { 0,0 };

	startButton->image->color = WHITE;
	startButton->transform.scale = { (int)(0.2f * pixelWidth),(int)(0.2f* pixelHeight) };
	startButton->transform.position.x = 0.5 * pixelWidth - 0.5 * startButton->transform.scale.x;
	startButton->transform.position.y = 0.5 * pixelHeight;
	startButton->text->color = BLACK;
	startButton->text->text = "START";
	startButton->AddClickEvent([this]() 
		{
			sceneManager.LoadScene("NewGame");
		}
	);



}

void MainMenuLevel::Update()
{

}

void MainMenuLevel::Unload()
{

}
