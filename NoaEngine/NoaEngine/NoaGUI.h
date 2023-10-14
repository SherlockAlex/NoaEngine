#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"
#include "Graphic.h"
#include "Actor.h"
#include "Transform.h"

namespace noa {

	//����
	typedef struct Font
	{
		std::shared_ptr<Sprite> sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
		Font()
		{

		}

		Font(std::shared_ptr<Sprite> sprite)
		{
			this->sprite = sprite;
			if (sprite) 
			{
				spriteGPU = std::make_shared<SpriteGPU>(sprite.get());
				spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
			}
		}

		Font(const SpriteFile & spriteFile)
		{
			this->sprite = std::make_shared<Sprite>(spriteFile, Vector<int>(64, 64));
			if (sprite) 
			{
				spriteGPU = std::make_shared<SpriteGPU>(sprite.get());
				spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
			}
		}

	}Font;

	class FontAsset {
	private:
		std::unordered_map<char, Font*> fonts;
	public:
		FontAsset(const char* ttfPath,int size);
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
		//��Ļλ��
		UITransform transform;
	protected:

		UIComponent();
		virtual ~UIComponent();

	public:
		virtual void Delete() = 0;

		virtual void Start() = 0;
		virtual void Update() = 0;

		void SetActive(bool active);
		bool GetActive();
	};

	class UICanvas : Actor
	{
	private:
		std::vector<UIComponent*> uiComponent;

	private:
		UICanvas(Scene * scene);
		~UICanvas() override;
	public:
		static UICanvas* Create(Scene* scene);

		void AddComponent(UIComponent* component);
		void SetActive(bool active) override;

		void Start() override;
		void Update() override;

	};

	class NoaButton;
	class NoaText;
	class NoaImage;

	class NoaText :public UIComponent 
	{
	public:
		std::string text = "text";
		//������ɫ
		uint32_t textColor = BLACK;
		//�����С
		int size = 25;
	
	protected:
		NoaText();
		~NoaText();

	public:

		static NoaText* Create();
		void Delete() override;

		void Start() override;
		void Update() override;

	};

	class NoaImage :public UIComponent {
	public:
		//��ȸ߶�
		Vector<int> scale = Vector<int>(150, 150);
		
		Sprite * sprite = nullptr;
		ColorRef color = WHITE;

	public:
		NoaImage();
		~NoaImage();

	public:

		static NoaImage* Create();
		void Delete() override;

		void Start() override;
		void Update() override;

	};

	class NoaButton :public UIComponent
	{
	public:
		//��ȸ߶�
		Vector<int> scale = Vector<int>(150, 50);
		//������
		std::string text = "BUTTON";
		//������ɫ
		uint32_t textColor = BLACK;
		
		uint32_t fontSize = 20;
		float fontNarrowX = 0.8f;

		//����������ɫ
		Sprite* sprite = nullptr;
		uint32_t normalColor = WHITE;
		uint32_t selectColor = LIGHTGRAY;
		//�����¼�
		NoaEvent<void> clickEvent;

	private:
		bool isSelect = false;
		ColorRef currentColor = normalColor;

	private:
		NoaButton();
		~NoaButton();

	public:
		static NoaButton* Create();
		void Delete() override;

		void SwapState();

		void Start() override;

		void Update() override;

		void AddClickEvent(std::function<void()> func);

	};
}

#endif // !NOAGUI_H


