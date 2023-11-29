#ifndef NOAENGINE_SLIDER_H
#define NOAENGINE_SLIDER_H

#include "UIComponent.h"

namespace noa {
	class Image;

	class Slider :public noa::UIComponent
	{
	private:
		UIBODY(Slider)
	private:
		Slider(noa::UIContainer* container);
		~Slider();

		void Start() override;
		void Update() override;
		void Render() override;

	public:
		Slider& SetLocalSize(int w,int h);
		Slider& SetAmount(float amount);
		Slider& SetTrackColor(uint32_t color);
		Slider& SetSliderColor(uint32_t color);
		Slider& SetAnchor(float x,float y);
		Slider* Apply();
	private:
		Image* track = nullptr;
		Image* slider = nullptr;

		// 表示滑块的宽度占了总宽度的多少
		float amount = 0.1f;

	};
}

#endif // !NOAENGINE_SLIDER_H

