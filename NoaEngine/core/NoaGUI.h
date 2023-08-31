#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"

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

	class NoaButton 
	{
	public:
		//屏幕位置
		Vector<int> position;
		//按键名
		string text;
		//宽度高度
		Vector<int> scale;
		//字体颜色
		uint32_t textColor;
		//按键常亮颜色
		uint32_t normalColor;
		uint32_t selectColor;
		//按键事件
		NoaEvent<void()> clickEvent;
	};
}

#endif // !NOAGUI_H


