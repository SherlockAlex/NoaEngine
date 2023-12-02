#pragma once
#include "NoaEngine.h"
class MainMenuUI:public noa::Actor
{
private:
	ACTOR(MainMenuUI)
private:
	MainMenuUI(noa::Scene* scene);
	~MainMenuUI();
	
	void CreateMainContainer();

private:
	noa::UIDocument* document
		= noa::UIDocument::Create(this)
		->SetID("document_mainmenu")
		.Apply();

};

