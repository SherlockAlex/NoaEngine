#pragma once
#include "Engine.h"
class MainMenuUI:public noa::Actor
{
private:
	ACTOR(MainMenuUI)
private:
	MainMenuUI(noa::Scene* scene);

	void Start() override;
	void Update() override;

private:
	noa::UIDocument* document =
		noa::UIDocument::Create(this)
		->SetID("main_menu_document")
		.Apply();

};

