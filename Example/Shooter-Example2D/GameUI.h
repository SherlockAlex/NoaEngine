#pragma once
#include "Engine.h"
class GameUI:public noa::Actor
{
private:
	ACTOR(GameUI)
private:
	GameUI(noa::Scene * scene);

	void Start() override;
public:
	template<class T>
	T* GetElementByID(const std::string& id)
	{
		if (document == nullptr)
		{
			return nullptr;
		}
		return document->GetElementByID<T>(id);
	}

private:
	noa::UIDocument* document = 
		noa::UIDocument::Create(this)
		->SetID("game_ui_document")
		.Apply();
};

