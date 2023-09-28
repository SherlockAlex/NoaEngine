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

void MainMenuLevel::Delete()
{
	delete this;
}

void MainMenuLevel::Awake()
{
	if (wolfResource == nullptr)
	{
		wolfResource = new WolfResource();
	}

	canvas = UICanvas::Create(this);
	background = NoaImage::Create();
	startButton = NoaButton::Create();

	background->sprite = &wolfResource->backgroundSprite;
	background->scale = { pixelWidth,pixelHeight };
	background->transform.position = { 0,0 };

	startButton->scale = { (int)(0.2 * pixelWidth),(int)(0.2 * pixelHeight) };
	startButton->transform.position.x = 0.5 * pixelWidth - 0.5 * startButton->scale.x;
	startButton->transform.position.y = 0.5 * pixelHeight;
	startButton->textColor = BLACK;
	startButton->text = "START";
	startButton->AddClickEvent([this]() 
		{
			sceneManager.LoadScene("NewGame");
		}
	);


	canvas->AddComponent(this->background);
	canvas->AddComponent(this->startButton);



}

void MainMenuLevel::Update()
{

}

void MainMenuLevel::Unload()
{

}
