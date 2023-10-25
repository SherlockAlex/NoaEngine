#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"
#include "Graphic.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "Transform.h"

namespace noa {

	//字体
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

	class UICanvas;
	class UICanvasComponent;

	class UIComponent
	{
	protected:
		bool active = false;
		friend class UICanvas;
		friend class UICanvasComponent;
	public:
		UITransform transform;
	protected:

		UIComponent(UICanvas * canvas);
		UIComponent(UICanvasComponent* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent *& ptr);

	protected:

		virtual void Start() = 0;
		virtual void Update() = 0;

	public:

		void SetActive(bool active);
		bool GetActive();
	};

	//Canvas作为独立的Actor存在
	class UICanvas final : Actor
	{
	private:
		NOBJECT(UICanvas)
	private:
		std::vector<UIComponent*> uiComponent;

	private:
		UICanvas(Scene * scene);
		~UICanvas() override;
	public:
		static UICanvas* Create(Scene* scene);

		void AddUIComponent(UIComponent* component);
		void SetActive(bool active) override;

		void Start() override;
		void Update() override;

	};

	//Canvas作为ActorComponent存在
	class UICanvasComponent final : ActorComponent
	{
	private:
		NOBJECT(UICanvasComponent)
	private:
		std::vector<UIComponent*> uiComponent;
	private:
		UICanvasComponent(Actor* actor);
		~UICanvasComponent() override;
	public:
		static UICanvasComponent* Create(Actor* actor);

		void AddUIComponent(UIComponent* component);
		void SetActive(bool active) override;

		void Start() override;
		void Update() override;

	};

	class Button;
	class Text;
	class Image;

	class Text :public UIComponent 
	{
	public:
		std::string text = "text";
		uint32_t color = BLACK;
		int size = 25;
		float narrow = 0.8f;

	protected:
		Text(UICanvas* canvas);
		Text(UICanvasComponent* canvas);
		~Text();

	public:

		static Text* Create(UICanvas * canvas);
		static Text* Create(UICanvasComponent* canvas);

		void Start() override;
		void Update() override;

	};
	
	class Image :public UIComponent {
	public:
		uint32_t color = WHITE;
		bool isFilpX = false;//进行图片翻转
	private:
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
	private:
		Image(UICanvas * canvas);
		Image(UICanvasComponent* canvas);
		~Image();

	public:

		static Image* Create(UICanvas * canvas);
		static Image* Create(UICanvasComponent * canvas);

		void SetSprite(Sprite * sprite);

		void Start() override;
		void Update() override;

	};

	class Button :public UIComponent
	{
	public:
		Text* text = nullptr;
		Image* image = nullptr;

		//按键常亮颜色
		uint32_t normalColor = WHITE;
		uint32_t selectColor = LIGHTGRAY;
		//按键事件
		NoaEvent<void> clickEvent;

	private:
		bool isSelect = false;
		uint32_t currentColor = normalColor;

	private:
		Button(UICanvas* canvas);
		Button(UICanvasComponent* canvas);
		~Button();

	public:
		static Button* Create(UICanvas * canvas);
		static Button* Create(UICanvasComponent* canvas);

		void SwapState();

		void Start() override;

		void Update() override;

		void AddClickEvent(std::function<void()> func);

	};
}

#endif // !NOAGUI_H


