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
	void Delete() override;

	void Awake() override;

	void Update() override;

	void Unload() override;

public:

	UICanvas* canvas = nullptr;
	NoaImage* background = nullptr;
	NoaButton* startButton = nullptr;

};

