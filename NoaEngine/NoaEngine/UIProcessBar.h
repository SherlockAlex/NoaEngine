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

	class ProcessBar :public UIComponent {
	private:
		ProcessBar(UIContainer* container);
		~ProcessBar();

		void UpdateRuntimeSprite();

		void Start() override;
		void Update() override;
		void Render() override;

	public:
		static ProcessBar* Create(UIContainer* container);
		ProcessBar& SetID(const std::string& id);
		ProcessBar& SetPosition(int x, int y);
		ProcessBar& SetAnchor(float x, float y);
		ProcessBar& SetActive(bool value);
		ProcessBar& SetBackgroundColor(uint32_t color);
		ProcessBar& SetFillColor(uint32_t color);
		ProcessBar& SetFinishedCallback(std::function<void()> func);
		ProcessBar& SetSize(int x, int y);
		ProcessBar& SetAmount(float amount);
		ProcessBar& SetRadius(int value);
		ProcessBar& SetInteractable(bool value);
		ProcessBar* Apply();

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

