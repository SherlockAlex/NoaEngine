#ifndef NOAENGINE_SLIDER_H
#define NOAENGINE_SLIDER_H

#include "UIComponent.h"

namespace noa {
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

	};
}

#endif // !NOAENGINE_SLIDER_H

