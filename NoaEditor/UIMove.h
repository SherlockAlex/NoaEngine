#pragma once
#include "NoaEditor.h"
class UIMove:public noa::UIScript
{
private:
	UIMove(noa::UIBody* body);
	~UIMove();

	void Update() override;

public:
	static UIMove* Create(noa::UIBody* body);

};

