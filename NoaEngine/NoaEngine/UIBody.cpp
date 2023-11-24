#include "UIBody.h"
#include "UIScript.h"
#include "InputSystem.h"
#include "Debug.h"

noa::UIBody::~UIBody() {
	for (auto& script : scripts)
	{
		script->Delete(script);
	}
	scripts.clear();
}

void noa::UIBody::OnUpdate() {
	for (auto& script : scripts)
	{
		script->Update();
	}
	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = 
		static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = 
		static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//Í¨³£×´Ì¬
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	handled = 
		mousePosX >= posX-2
		&& mousePosX <= posX + transform.size.x+2
		&& mousePosY >= posY-2
		&& mousePosY <= posY + transform.size.y+2;

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
		static_cast<int>(x 
			+ anchor.x * transform.size.x
		- fatherTransform.position.x);

	transform.position.y =
		static_cast<int>(y 
			+ anchor.y * transform.size.y
		- fatherTransform.position.y);
}