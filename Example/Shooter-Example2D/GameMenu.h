#pragma once
#include "Engine.h"
class GameMenu:public noa::Actor
{
private:
	ACTOR(GameMenu)
private:
	GameMenu(noa::Scene* scene);
	void Start() override;

public:
	void Show();
	void Close();

	template<class T>
	T* GetElementByID(const std::string& id) 
	{
		if (document==nullptr) 
		{
			return nullptr;
		}
		return document->GetElementByID(id);
	}

private:
	noa::UIDocument* document = noa::UIDocument::Create(this);
	noa::UIContainer* container =
		noa::UIContainer::Create(document)
		->SetID("game_menu_document")
		.SetLocalSize(noa::Screen::width, noa::Screen::height)
		.SetAnchor(0.5f, 0.5f)
		.Apply();
};

