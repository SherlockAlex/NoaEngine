#pragma once
#include "GameEngine.h"

using namespace noa;

class MainMenuLevel :public Scene
{
private:
	MainMenuLevel();
	~MainMenuLevel();

public:
	static MainMenuLevel* Create();

	void Awake() override;

	void Update() override;

	void Unload() override;

public:

	UICanvas* canvas = nullptr;
	Image* background = nullptr;
	Button* startButton = nullptr;

};

