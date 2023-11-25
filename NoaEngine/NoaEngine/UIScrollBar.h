#ifndef NOAENGINE_PROCESSBAR_H
#define NOAENGINE_PROCESSBAR_H

#include <functional>

#include "Sprite.h"
#include "Graphic.h"
#include "NoaAction.h"
#include "UIComponent.h"
#include "UIContainer.h"

namespace noa {
	class Image;

	class ScrollBar :public UIComponent {
	private:
		ScrollBar(UIContainer* container);
		~ScrollBar();

		void UpdateRuntimeSprite();

		void Start() override;
		void Update() override;
		void Render() override;

	public:
		static ScrollBar* Create(UIContainer* container);
		ScrollBar& SetID(const std::string& id);
		ScrollBar& SetLocalPosition(int x, int y);
		ScrollBar& SetGlobalPosition(int x,int y);
		ScrollBar& SetAnchor(float x, float y);
		ScrollBar& SetActive(bool value);
		ScrollBar& SetBackgroundColor(uint32_t color);
		ScrollBar& SetFillColor(uint32_t color);
		ScrollBar& SetFinishedCallback(std::function<void()> func);
		ScrollBar& SetSize(int x, int y);
		ScrollBar& SetAmount(float amount);
		ScrollBar& SetRadius(int value);
		ScrollBar& SetInteractable(bool value);
		ScrollBar& SetActiveScale(float value);
		ScrollBar* Apply();

		float GetValue();

	private:
		uint32_t backgroundColor = noa::BLACK;
		uint32_t fillColor = noa::WHITE;

		Image* background = nullptr;
		Image* runtime = nullptr;

		Sprite fillbar;

		bool interactable = true;

		NoaEvent<void> finishedEvent;
		float amount = 0.0f;
		float oldAmount = amount;
		bool finished = false;
		int radius = 0;
	};
}

#endif // !NOAENGINE_PROCESSBAR_H

