#ifndef NOAENGINE_BUTTON_H
#define NOAENGINE_BUTTON_H

#include <memory>
#include <functional>

#include "Graphic.h"
#include "NoaAction.h"
#include "NoaMath.h"
#include "UIComponent.h"
#include "UIContainer.h"

namespace noa {

	class AudioClip;
	class Sprite;
	class Label;
	class Image;

	class Button :public UIComponent
	{
	private:
		Button(UIContainer* group);
		~Button();

		void SwapState();

		void Start() override;
		void Update() override;
		void Render() override;

	public:
		static Button* Create(UIContainer* group);

		// ���ư�ť���ԣ�����������ť���¼���id��λ��
		Button& Clone(Button* button);
		Button& SetID(const std::string& id);
		Button& SetNormalColor(uint32_t color);
		Button& SetHeightLightColor(uint32_t color);
		Button& SetClickColor(uint32_t color);
		Button& SetPosition(int x, int y);
		Button& SetAnchor(float x, float y);
		Button& SetActive(bool value);
		Button& SetSize(int w, int h);
		Button& SetSprite(noa::Sprite* sprite);
		Button& SetRadius(int value);
		Button& SetTextOffset(float x, float y);
		Button& SetText(const std::wstring& text);
		Button& SetTextNormalColor(uint32_t color);
		Button& SetTextHeightLightColor(uint32_t color);
		Button& SetTextClickColor(uint32_t color);
		Button& SetFontSize(uint32_t size);
		Button& SetNormalScale(float value);
		Button& SetHeightLightScale(float value);
		Button& SetClickScale(float value);
		Button& SetSelectedAudio(std::shared_ptr<noa::AudioClip> audio);
		Button& SetClickAudio(std::shared_ptr<noa::AudioClip> audio);
		Button& AddSelectedCallback(std::function<void()> func);
		Button& AddClickCallback(std::function<void()> func);
		Button* Apply();

	public:

		//����������ɫ
		uint32_t normalColor = LIGHTGRAY;
		uint32_t heightLightColor = WHITE;
		uint32_t clickColor = DARKGRAY;

		//����������ɫ
		uint32_t textNormalColor = BLACK;
		uint32_t textHeightLightColor = BLACK;
		uint32_t textClickColor = BLACK;

		//���ð�ť��С
		float normalScale = 1.0f;
		float heightLightScale = 1.0f;
		float clickScale = 1.0f;

	private:
		Sprite* sprite = nullptr;

		Label* label = nullptr;
		Image* image = nullptr;

		bool isClickReady = false;
		bool isSelect = false;
		bool selectEventFlag = false;
		uint32_t currentColor = normalColor;
		uint32_t currentTextColor = textNormalColor;
		float targetScale = normalScale;
		float currentScale = normalScale;
		int radius = 25;

		uint32_t fontSize = 0;

		Vector<float> labelOffset;
		Vector<float> currentSize;

		//�����¼�
		NoaEvent<void> selectedEvent;
		NoaEvent<void> clickEvent;

		std::shared_ptr<noa::AudioClip> selectedAudio = nullptr;
		std::shared_ptr<noa::AudioClip> clickAudio = nullptr;


		friend class Button;
	};
}

#endif // !NOAENGINE_BUTTON_H

