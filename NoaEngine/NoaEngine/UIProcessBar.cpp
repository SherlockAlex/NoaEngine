#include "UIProcessBar.h"
#include "UIImage.h"
#include "InputSystem.h"

noa::ProcessBar::ProcessBar(noa::UIContainer* container)
	:noa::UIComponent(container)
{
	background = noa::Image::Create(container);
	runtime = noa::Image::Create(container);
	SetSize(360, 20);
}

noa::ProcessBar::~ProcessBar() {

}

void noa::ProcessBar::UpdateRuntimeSprite() {
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

void noa::ProcessBar::Start() {

}

void noa::ProcessBar::Update()
{

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//通常状态
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	//交互
	//获取鼠标的位置和自己当前的global位置

	if (interactable
		&& Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	{
		Vector<double> mousePos = Input::GetMousePosition();
		const int mousePosX = static_cast<int>(mousePos.x);
		const int mousePosY = static_cast<int>(mousePos.y);
		if (mousePosX >= posX - 10 && mousePosX <= posX + transform.size.x + 10
		&& mousePosY >= posY - 5 && mousePosY <= posY + transform.size.y + 5
		)
		{
			//鼠标在范围内
			const float deltaX = static_cast<float>(mousePosX - posX);
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

void noa::ProcessBar::Render() {
	//绘制

	background->anchor = anchor;
	background->transform.position = transform.position;
	background->transform.size = transform.size;
	background->color = backgroundColor;

	runtime->anchor = anchor;
	runtime->transform.position = transform.position;
	runtime->transform.size = transform.size;
	runtime->color = fillColor;

}

noa::ProcessBar* noa::ProcessBar::Create(
	noa::UIContainer* container)
{
	return new noa::ProcessBar(container);
}

noa::ProcessBar& noa::ProcessBar::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetPosition(int x, int y)
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetAnchor(float x, float y)
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetActive(bool value)
{
	noa::UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetBackgroundColor(
	uint32_t color)
{
	this->backgroundColor = color;
	background->SetColor(color);
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetFillColor(
	uint32_t color) {
	this->fillColor = color;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetFinishedCallback(
	std::function<void()> func)
{
	this->finishedEvent += func;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetSize(int x, int y)
{
	transform.size.x = x;
	transform.size.y = y;

	fillbar.w = x;
	fillbar.h = y;
	fillbar.ResizeAndFull(x, y, noa::RGBA(0, 0, 0, 0));

	background->SetSize(x, y).Apply();
	runtime->SetSize(x, y).SetSprite(&fillbar).Apply();

	return *this;

}

noa::ProcessBar& noa::ProcessBar::SetAmount(float amount)
{
	this->amount = amount;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetRadius(int value)
{

	this->radius = value;

	background->SetRadius(value).Apply();
	runtime->SetRadius(value).Apply();

	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetInteractable(bool interactable)
{
	this->interactable = interactable;
	return *this;
}

noa::ProcessBar* noa::ProcessBar::Apply() {
	return this;
}

float noa::ProcessBar::GetValue() {
	return this->amount;
}


