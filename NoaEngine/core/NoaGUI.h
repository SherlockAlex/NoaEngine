#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"
#include "Graphic.h"
#include "Behaviour.h"

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

	class UIComponent: public Behaviour
	{
	public:
		//屏幕位置
		Vector<int> position = Vector<int>(0.0, 0.0);
	public:
		UIComponent();
		~UIComponent();
	};

	class NoaCanvase 
	{
	public:
		vector<UIComponent*> uiComponent;

	public:
		NoaCanvase();
		~NoaCanvase();

		void SetActive(bool active);

	};

	class NoaButton;
	class NoaText;

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


