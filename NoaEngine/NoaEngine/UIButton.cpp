#include "UIButton.h"
#include "UILabel.h"
#include "UIImage.h"
#include "InputSystem.h"
#include "AudioSystem.h"

noa::Button::Button(UIContainer* group) :UIComponent(group)
{
	image = Image::Create(group);
	label = Label::Create(group);

	label->anchor = { 0.5f,0.5f };

	SetFontSize(20);
	SetPosition(0, 0);
	SetSize(240, 60);
	SetNormalColor(noa::RGBA(255, 255, 255, 255));
	SetHeightLightColor(noa::RGBA(255, 0, 0, 255));
	SetAnchor(0.5f, 0.5f);
	SetTextOffset(0.0f, 0.0f);
	SetTextNormalColor(noa::BLACK);
	SetTextHeightLightColor(noa::WHITE);
	SetTextClickColor(noa::BLACK);
	SetNormalScale(1.0f);
	SetHeightLightScale(1.1f);
	SetClickScale(0.9f);

	Apply();

}

noa::Button::~Button()
{

}

noa::Button* noa::Button::Create(UIContainer* group)
{
	Button* button = new Button(group);
	return button;
}

void noa::Button::SwapState()
{

	//button的状态转换函数

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//通常状态
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	isClickReady = false;
	if (mousePosX >= posX && mousePosX <= posX + transform.size.x * currentScale
		&& mousePosY >= posY && mousePosY <= posY + transform.size.y * currentScale
		)
	{
		isSelect = true;//进入到被高亮状态

		if (!selectEventFlag)
		{
			//高亮事件
			if (this->selectedAudio != nullptr)
			{
				selectedAudio->Play(false);
			}
			this->selectedEvent.Invoke();
			selectEventFlag = true;
		}

		if (Input::GetMouseKeyHold(MouseButton::LEFT_BUTTON))
		{
			//进入点击状态
			isClickReady = true;
		}
		else if (Input::GetMouseKeyUp(MouseButton::LEFT_BUTTON))
		{
			if (clickAudio != nullptr)
			{
				clickAudio->Play(false);
			}
			this->clickEvent.Invoke();
		}


	}
	else {
		isSelect = false;
		selectEventFlag = false;
	}

	if (!isClickReady)
	{
		if (isSelect)
		{
			currentColor = heightLightColor;
			currentTextColor = textHeightLightColor;
			targetScale = heightLightScale;
		}
		else {
			currentColor = normalColor;
			currentTextColor = textNormalColor;
			targetScale = normalScale;
		}
	}
	else {
		currentColor = clickColor;
		currentTextColor = textClickColor;
		targetScale = clickScale;
	}

}

void noa::Button::Start()
{

}

void noa::Button::Update()
{
	this->SwapState();
}

void noa::Button::Render() {

	const float lerpSpeed = 2.0f;
	currentScale = noa::Math::LinearLerp(
		currentScale
		, targetScale
		, lerpSpeed
	);

	currentSize.x = transform.size.x * currentScale;
	currentSize.y = transform.size.y * currentScale;

	this->label->SetFontSize(static_cast<uint32_t>(fontSize * currentScale));

	image->anchor = anchor;
	image->transform.position = transform.position;
	image->transform.size.x = static_cast<int>(currentSize.x);
	image->transform.size.y = static_cast<int>(currentSize.y);
	image->color = currentColor;

	label->anchor = anchor;
	label->color = currentTextColor;
	label->transform.position.x =
		static_cast<int>(transform.position.x
			+ transform.size.x * labelOffset.x);
	label->transform.position.y =
		static_cast<int>(transform.position.y
			+ transform.size.y * labelOffset.y);
}

noa::Button& noa::Button::Clone(Button* button) {
	return this->SetNormalColor(button->normalColor)
		.SetHeightLightColor(button->heightLightColor)
		.SetClickColor(button->clickColor)
		.SetSize(button->transform.size.x,
									button->transform.size.y)
		.SetRadius(button->radius)
		.SetTextNormalColor(button->textNormalColor)
		.SetTextHeightLightColor(button->textHeightLightColor)
		.SetTextClickColor(button->textClickColor)
		.SetFontSize(button->fontSize)
		.SetNormalScale(button->normalScale)
		.SetHeightLightScale(button->heightLightScale)
		.SetAnchor(button->anchor.x, button->anchor.y)
		.SetTextOffset(button->labelOffset.x, button->labelOffset.y)
		.SetClickAudio(button->clickAudio)
		.SetSelectedAudio(button->selectedAudio)
		.SetSprite(button->sprite)
		.SetClickScale(button->clickScale);
}

noa::Button& noa::Button::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::Button& noa::Button::SetNormalColor(uint32_t color)
{
	this->normalColor = color;
	return *this;
}

noa::Button& noa::Button::SetHeightLightColor(uint32_t color)
{
	this->heightLightColor = color;
	return *this;
}

noa::Button& noa::Button::SetClickColor(uint32_t color)
{
	this->clickColor = color;
	return *this;
}

noa::Button& noa::Button::SetPosition(int x, int y)
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Button& noa::Button::SetAnchor(float x, float y)
{
	anchor.x = x;
	anchor.y = y;
	return *this;
}

noa::Button& noa::Button::SetActive(bool value)
{
	UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::Button& noa::Button::SetSize(int w, int h)
{
	this->transform.size.x = w;
	this->transform.size.y = h;

	this->currentSize.x = static_cast<float>(transform.size.x);
	this->currentSize.y = static_cast<float>(transform.size.y);

	this->image->SetSize(w, h).Apply();

	return *this;
}

noa::Button& noa::Button::SetSprite(noa::Sprite* sprite)
{
	this->sprite = sprite;
	this->image->SetSprite(sprite).Apply();
	return *this;
}

noa::Button& noa::Button::SetRadius(int value)
{
	//四个角的半径
	this->radius = value;
	this->image->SetRadius(value).Apply();
	return *this;
}

noa::Button& noa::Button::SetTextOffset(float x, float y)
{
	labelOffset.x = x;
	labelOffset.y = y;
	return *this;
}

noa::Button& noa::Button::SetText(const std::wstring& text)
{
	this->label->SetText(text);
	return *this;
}

noa::Button& noa::Button::SetTextNormalColor(uint32_t color)
{
	this->textNormalColor = color;
	return *this;
}

noa::Button& noa::Button::SetTextHeightLightColor(uint32_t color)
{
	this->textHeightLightColor = color;
	return *this;
}

noa::Button& noa::Button::SetTextClickColor(uint32_t color)
{
	this->textClickColor = color;
	return *this;
}

noa::Button& noa::Button::SetFontSize(uint32_t size)
{
	this->fontSize = size;
	return *this;
}

noa::Button& noa::Button::SetNormalScale(float value)
{
	this->normalScale = value;
	return *this;
}

noa::Button& noa::Button::SetHeightLightScale(float value)
{
	this->heightLightScale = value;
	return *this;
}

noa::Button& noa::Button::SetClickScale(float value)
{
	this->clickScale = value;
	return *this;
}

noa::Button& noa::Button::SetSelectedAudio(
	std::shared_ptr<noa::AudioClip> audio)
{
	this->selectedAudio = audio;
	return *this;
}

noa::Button& noa::Button::SetClickAudio(
	std::shared_ptr<noa::AudioClip> audio)
{
	this->clickAudio = audio;
	return *this;
}

noa::Button& noa::Button::AddSelectedCallback(
	std::function<void()> func
)
{
	this->selectedEvent += func;
	return *this;
}


noa::Button& noa::Button::AddClickCallback(
	std::function<void()> func
)
{
	this->clickEvent += func;
	return *this;
}

noa::Button* noa::Button::Apply() {
	this->image->Apply();
	return this;
}