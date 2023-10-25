#pragma once
#include "GameEngine.h"
class Enter:public noa::Actor
{
private:
	NOBJECT(Enter)
private:
	std::string nextScene = "";
private:
	Enter(noa::Scene * scene,const std::string & nextScene):Actor(scene)
	{
		this->nextScene = nextScene;
		this->tag = "Enter";
	}
public:
	void SetPosition(float x,float y) 
	{
		this->transform.position = { x,y };
	}

	void Load() 
	{
		noa::sceneManager.LoadScene(nextScene);
	}

};

