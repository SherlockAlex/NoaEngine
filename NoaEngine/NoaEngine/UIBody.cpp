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
		static_cast<int>(
			transform.position.x 
			+ fatherTransform.position.x 
			);

	const int posY = 
		static_cast<int>(
			transform.position.y 
			+ fatherTransform.position.y 
			);
	
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	handled = 
		mousePosX >= posX - anchor.x*transform.size.x
		&& mousePosX <= posX + transform.size.x - anchor.x * transform.size.x
		&& mousePosY >= posY - anchor.y * transform.size.y
		&& mousePosY <= posY + transform.size.y - anchor.y * transform.size.y;

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