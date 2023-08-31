#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"

namespace noa {

	//����
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
		//��Ļλ��
		Vector<int> position;
		//������
		string text;
		//��ȸ߶�
		Vector<int> scale;
		//������ɫ
		uint32_t textColor;
		//����������ɫ
		uint32_t normalColor;
		uint32_t selectColor;
		//�����¼�
		NoaEvent<void()> clickEvent;
	};
}

#endif // !NOAGUI_H


