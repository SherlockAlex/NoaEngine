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
				spriteGPU = SpriteGPU::Create(sprite.get());
				spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
			}
		}

		Font(const SpriteFile & spriteFile)
		{
			this->sprite = std::make_shared<Sprite>(spriteFile, Vector<int>(64, 64));
			if (sprite) 
			{
				spriteGPU = SpriteGPU::Create(sprite.get());
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
	class Label;
	class Image;

	class Label :public UIComponent
	{
	public:
		std::string text = "text";
		uint32_t color = BLACK;
		uint32_t size = 25;
		float narrow = 0.8f;

	protected:
		Label(UICanvas* canvas);
		Label(UICanvasComponent* canvas);
		~Label();

	public:

		static Label* Create(UICanvas * canvas);
		static Label* Create(UICanvasComponent* canvas);

		Label& SetFontSize(uint32_t size);
		Label& SetPosition(int x,int y);
		Label& SetScale(int w,int h);
		Label* Apply();

		void Start() override;
		void Update() override;

	};
	
	enum class ImageStyle 
	{
		DEFAULT,
		COVER
	};

	class Image :public UIComponent {
	public:
		ImageStyle style = ImageStyle::DEFAULT;
		uint32_t color = WHITE;
		bool isFilpX = false;
	private:
		Sprite* sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
	private:
		Image(UICanvas * canvas);
		Image(UICanvasComponent* canvas);
		~Image();

	public:

		static Image* Create(UICanvas * canvas);
		static Image* Create(UICanvasComponent * canvas);

		Image& SetStyle(ImageStyle style);
		Image & SetSprite(Sprite * sprite);
		Image* Apply();

		void Start() override;
		void Update() override;

	};

	class Button :public UIComponent
	{
	public:
		Label* label = nullptr;
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

		Button& SetPosition(int x,int y);
		Button& SetScale(int w,int h);
		Button& SetSprite(Sprite * sprite);
		Button& SetText(const std::string & text);
		Button& SetFontSize(uint32_t size);
		Button& AddClickEvent(std::function<void()> func);
		Button* Apply();
	};
}

#endif // !NOAGUI_H


