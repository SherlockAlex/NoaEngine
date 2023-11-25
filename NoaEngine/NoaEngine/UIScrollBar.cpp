#include "UIScrollBar.h"
#include "UIImage.h"
#include "InputSystem.h"

noa::ScrollBar::ScrollBar(noa::UIContainer* container)
	:noa::UIComponent(container)
{
	background = noa::Image::Create(container);
	runtime = noa::Image::Create(container);
	SetSize(360, 20);
}

noa::ScrollBar::~ScrollBar() {

}

void noa::ScrollBar::UpdateRuntimeSprite() {
	//首先计算x进度
	const int targetX = static_cast<int>(amount * transform.size.x);
	fillbar.Full(noa::RGBA(0, 0, 0, 0));
	for (int x = 0; x < targetX; x++)
	{
		for (int y = 0; y < fillbar.h; y++)
		{
			fillbar.SetPixelColor(
				x, y
				, noa::RGBA(255, 255, 255, 255));
		}
	}
}

void noa::ScrollBar::Start() {

}

void noa::ScrollBar::Update()
{
	noa::UIBody::OnUpdate();

	if (interactable
		&& Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	{
		Vector<double> mousePos 
			= Input::GetMousePosition();
		const int mousePosX 
			= static_cast<int>(mousePos.x);
		const int mousePosY 
			= static_cast<int>(mousePos.y);
		
		const int posX = 
			static_cast<int>(
				globalTransform.position.x
				-2*anchor.x*transform.size.x
				);
		const int posY = 
			static_cast<int>(
				globalTransform.position.y
				-2*anchor.y*transform.size.y
				);

		if (handled)
		{

			const float deltaX = 
				static_cast<float>(mousePosX - posX);
			amount = deltaX / transform.size.x;
			if (amount <= 0.0f)
			{
				amount = 0.0f;
			}
			else if (amount >= 1.0f) {
				amount = 1.0f;
			}
		}

	}

	if (amount >= 1.0f)
	{
		amount = 1.0f;
		if (!finished)
		{
			finishedEvent();
			finished = true;
		}
	}
	else if (amount < 1.0f)
	{
		finished = false;
	}

	//更新数据
	if (oldAmount == amount)
	{
		return;
	}

	UpdateRuntimeSprite();
	SetRadius(radius);
	runtime->SetSprite(&fillbar).Apply();
	oldAmount = amount;

}

void noa::ScrollBar::Render() {
	//绘制

	background->SetAnchor(anchor.x,anchor.y);
	background->SetLocalPosition(transform.position.x,transform.position.y);
	background->SetLocalSize(transform.size.x,transform.size.y);
	background->color = backgroundColor;

	runtime->SetAnchor(anchor.x, anchor.y);
	runtime->SetLocalPosition(transform.position.x,transform.position.y);
	runtime->SetSize(transform.size.x,transform.size.y);
	runtime->color = fillColor;
}

noa::ScrollBar* noa::ScrollBar::Create(
	noa::UIContainer* container)
{
	return new noa::ScrollBar(container);
}

noa::ScrollBar& noa::ScrollBar::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetLocalPosition(int x, int y)
{
	noa::UIBody::SetLocalPosition(x,y);
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetGlobalPosition(int x,int y)
{
	noa::UIBody::SetGlobalPosition(x,y);
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetAnchor(float x, float y)
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetActive(bool value)
{
	noa::UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetBackgroundColor(
	uint32_t color)
{
	this->backgroundColor = color;
	background->SetColor(color);
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetFillColor(
	uint32_t color) {
	this->fillColor = color;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetFinishedCallback(
	std::function<void()> func)
{
	this->finishedEvent += func;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetSize(int x, int y)
{
	noa::UIBody::SetLocalSize(x, y);

	fillbar.w = x;
	fillbar.h = y;
	fillbar.ResizeAndFull(x, y, noa::RGBA(0, 0, 0, 0));

	background->SetSize(x, y).Apply();
	runtime->SetSize(x, y).SetSprite(&fillbar).Apply();

	return *this;

}

noa::ScrollBar& noa::ScrollBar::SetAmount(float amount)
{
	this->amount = amount;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetRadius(int value)
{

	this->radius = value;

	background->SetRadius(value).Apply();
	runtime->SetRadius(value).Apply();

	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetInteractable(bool interactable)
{
	this->interactable = interactable;
	return *this;
}

noa::ScrollBar& noa::ScrollBar::SetActiveScale(float value)
{
	noa::UIBody::SetActiveScale(value);
	return *this;
}

noa::ScrollBar* noa::ScrollBar::Apply() {
	return this;
}

float noa::ScrollBar::GetValue() {
	return this->amount;
}


