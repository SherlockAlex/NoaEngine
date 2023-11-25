#pragma once
#include "Engine.h"
class GameUI:public noa::Actor
{
private:
	ACTOR(GameUI)
private:
	GameUI(noa::Scene * scene);

	void Start() override;

private:
	noa::UIDocument* document = 
		noa::UIDocument::Create(this)
		->SetID("game_ui_document")
		.Apply();
};

