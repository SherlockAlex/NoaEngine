#include "UIBody.h"
#include "UIScript.h"
#include "Input.h"
#include "Debug.h"

noa::UIBody::~UIBody() {
	for (auto& script : scripts)
	{
		script->Delete(script);
	}
	scripts.clear();
}

void noa::UIBody::OnUpdate() {
	
	
	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX =
		static_cast<int>(
			transform.position.x
			+ fatherTransform.position.x
			+ anchor.x * transform.size.x
			);

	const int posY = 
		static_cast<int>(
			transform.position.y 
			+ fatherTransform.position.y
			+ anchor.y * transform.size.y
			);
	
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	const float minX = 
		static_cast<float>(
			posX
			-2*anchor.x
			*transform.size.x);
	const float maxX = 
		static_cast<float>(
			posX
			+transform.size.x 
			- 2*anchor.x 
			* transform.size.x);
	const float minY = 
		static_cast<float>(
			posY
			-2*anchor.y 
			* transform.size.y);
	const float maxY = 
		static_cast<float>(
			posY
			+transform.size.y 
			- 2*anchor.y 
			* transform.size.y);

	const float deltaX = 
		(activeScale - 1) * transform.size.x;
	const float deltaY =
		(activeScale - 1) * transform.size.y;

	//noa::Vector<float> scaleX = {minX,maxX};
	//noa::Debug::Log(noa::ToString<float>(scaleX));

	//全屏的时候，鼠标位置是1500*1080
	//但是图像还是1920，1080

	handled = 
		mousePosX >= minX - deltaX
		&& mousePosX <= maxX + deltaX
		&& mousePosY >= minY - deltaY
		&& mousePosY <= maxY + deltaY;
	
	if (handled&&!isEnter&&!isEnterFlag) 
	{
		isEnter = true;
		isEnterFlag = true;
	}
	else if (!handled&&!isExit&&!isExitFlag)
	{
		isExit = true;
		isExitFlag = true;
	}

	for (auto& script : scripts)
	{
		script->Update();
		if (isEnter) 
		{
			script->OnEnter();
			isEnter = false;
			isExitFlag = false;
		}
		if (handled) 
		{
			script->OnStay();
		}
		if (isExit)
		{
			script->OnExit();
			isExit = false;
			isEnterFlag = false;
		}
	}

}

void noa::UIBody::AddScript(noa::UIScript* script) 
{
	if (script == nullptr)
	{
		return;
	}
	this->scripts.push_back(script);
}

bool noa::UIBody::GetHandle() {
	return this->handled;
}

void noa::UIBody::SetLocalPosition(int x, int y)
{
	this->transform.position.x = x;
	this->transform.position.y = y;
}

void noa::UIBody::SetLocalSize(int w,int h)
{
	this->transform.size.x = w;
	this->transform.size.y = h;
}

void noa::UIBody::SetFatherTransform(
	const UITransform& transform)
{
	this->fatherTransform = transform;
}

void noa::UIBody::SetGlobalPosition(int x, int y)
{
	transform.position.x = 
		x - fatherTransform.position.x;
	transform.position.y = 
		y - fatherTransform.position.y;
}

void noa::UIBody::SetFatherAnchor(float x, float y) {
	fatherAnchor.x = x;
	fatherAnchor.y = y;
}

void noa::UIBody::SetActiveScale(float value)
{
	this->activeScale = value;
}