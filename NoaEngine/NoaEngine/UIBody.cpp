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

	const float minX = 
		static_cast<float>(posX - anchor.x * transform.size.x);
	const float maxX = 
		static_cast<float>(posX + transform.size.x
		- anchor.x * transform.size.x);
	const float minY = 
		static_cast<float>(posY - anchor.y * transform.size.y);
	const float maxY = 
		static_cast<float>(posY + transform.size.y
		- anchor.y * transform.size.y);

	handled = 
		mousePosX >= minX
		&& mousePosX <= maxX
		&& mousePosY >= minY
		&& mousePosY <= maxY;

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