#include "UISlider.h"
#include "UIImage.h"
#include "InputSystem.h"

noa::Slider::Slider(noa::UIContainer* container):
	noa::UIComponent(container)
{
	track = noa::Image::Create(container);
	slider = noa::Image::Create(container);
}

noa::Slider::~Slider() 
{

}

void noa::Slider::Start() {

}

void noa::Slider::Update() {
	//��д�����߼�
	if (handled) 
	{
		if (noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON)) 
		{
			//��������϶������λ��
		}
	}
}

void noa::Slider::Render() {

}

noa::Slider& noa::Slider::SetLocalSize(int w,int h) 
{
	noa::UIBody::SetLocalSize(w,h);
	//����Ĵ�СӦ�������Ĳ����ٷֱ�������
	track->SetLocalSize(w,h);
	return *this;
}

noa::Slider& noa::Slider::SetAmount(float amount) 
{
	if (amount<0.0f) 
	{
		this->amount = 0.0f;
	}
	else if (amount>1.0f)
	{
		this->amount = 1.0f;
	}
	else {
		this->amount = amount;
	}

	//���㻬��ĸ߶ȴ�С(Ĭ����������)
	const int sliderWidth = this->transform.size.x;
	const int sliderHeight = 
		static_cast<int>(this->amount * this->transform.size.y);

	this->slider->SetLocalSize(sliderWidth,sliderHeight);

	return *this;

}

noa::Slider& noa::Slider::SetTrackColor(uint32_t color)
{
	this->track->SetColor(color);
	return *this;
}

noa::Slider& noa::Slider::SetSliderColor(uint32_t color)
{
	this->slider->SetColor(color);
	return *this;
}

noa::Slider& noa::Slider::SetAnchor(float x,float y) 
{
	this->slider->SetAnchor(x,y);
	this->track->SetAnchor(x,y);
	return *this;
}

noa::Slider* noa::Slider::Apply() {
	return this;
}