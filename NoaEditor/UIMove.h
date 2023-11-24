#pragma once
#include "NoaEditor.h"
class UIMove:public noa::UIScript
{
private:
	UISCRIPT(UIMove)
private:
	UIMove(noa::UIBody* body);
	~UIMove();

	void Update() override;

};

