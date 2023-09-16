#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"
#include "Graphic.h"
#include "Actor.h"

namespace noa {

	//字体
	typedef struct Font
	{
		Sprite* sprite = nullptr;

		Font()
		{

		}

		Font(Sprite* sprite)
		{
			this->sprite = sprite;
		}

		Font(SpriteFile spriteFile)
		{
			this->sprite = new Sprite(spriteFile, Vector<int>(1.0, 1.0));
		}

	}Font;

	class FontAsset {
	private:
		unordered_map<char, Font*> fonts;
	public:
		FontAsset(const char * fontPath);
		Font* GetFont(char c);

		Font* operator [](char c)
		{
			return GetFont(c);
		}

	};

	class UIComponent
	{
	protected:
		bool isActive = false;

	public:
		//屏幕位置
		Vector<int> position = Vector<int>(0.0, 0.0);
	public:

		UIComponent();
		~UIComponent();

		virtual void Start() = 0;
		virtual void Update() = 0;

		void SetActive(bool active);
		bool GetActive();
	};

	class NoaCanvase : Actor
	{
	private:
		vector<UIComponent*> uiComponent;

	public:
		NoaCanvase();
		~NoaCanvase();

		void AddComponent(UIComponent* component);
		void SetActive(bool active);

		void Start() override;
		void Update() override;

	};

	class NoaButton;
	class NoaText;
	class NoaImage;

	class NoaText :public UIComponent {
	public:
		string text = "text";
		//字体颜色
		uint32_t textColor = BLACK;
		//字体大小
		int size = 25;
	
	public:
		NoaText();
		~NoaText();

		void Start() override;
		void Update() override;

	};

	class NoaImage :public UIComponent {
	public:
		//宽度高度
		Vector<int> scale = Vector<int>(150, 150);
		
		Sprite * sprite = nullptr;
		ColorRef color = WHITE;

	public:
		NoaImage();
		~NoaImage();

		void Start() override;
		void Update() override;

	};

	class NoaButton :public UIComponent
	{
	public:
		//宽度高度
		Vector<int> scale = Vector<int>(150, 50);
		//按键名
		string text = "BUTTON";
		//字体颜色
		uint32_t textColor = BLACK;
		
		//按键常亮颜色
		Sprite* sprite = nullptr;
		uint32_t normalColor = WHITE;
		uint32_t selectColor = LIGHTGRAY;
		//按键事件
		NoaEvent<void()> clickEvent;

	private:
		bool isSelect = false;
		ColorRef currentColor = normalColor;

	public:
		NoaButton();
		~NoaButton();

		void SwapState();

		void Start() override;

		void Update() override;

		void AddClickEvent(function<void()> func);

	};
}

#endif // !NOAGUI_H


