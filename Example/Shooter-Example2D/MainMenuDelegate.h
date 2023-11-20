#pragma once
#include "Engine.h"

class MainMenuDelegate {
	//用于定义MainMenuScene的行为
public:
	static void OnLoad(noa::Scene * scene);
	static void OnUpdate(noa::Scene* scene);
};
