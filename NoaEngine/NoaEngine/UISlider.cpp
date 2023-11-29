#include "UISlider.h"
#include "UIImage.h"

noa::Slider::Slider(noa::UIContainer* container):
	noa::UIComponent(container)
{
	bar = noa::Image::Create(container);
	slider = noa::Image::Create(container);
}

noa::Slider::~Slider() 
{

}

void noa::Slider::Start() {

}

void noa::Slider::Update() {

}

void noa::Slider::Render() {

}