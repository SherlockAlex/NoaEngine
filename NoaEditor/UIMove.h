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

	void OnEnter() override;
	void OnStay() override;
	void OnExit() override;

private:
	bool isHold = false;
};

