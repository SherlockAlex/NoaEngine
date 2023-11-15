#ifndef NOAGUI_H
#define NOAGUI_H

#include <stack>

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"
#include "Graphic.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "Transform.h"

namespace noa {

	class Button;
	class Label;
	class Image;
	class UIGroup;

	//字体
	typedef struct Font
	{
		int w = 0;
		int h = 0;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

		noa::Vector<int> bearing;	//从基准线到字形左部/顶部的偏移值
		uint32_t advance = 0;			//原点距下一个字形原点的距离

		Font()
		{

		}

		Font(const SpriteFile & spriteFile)
		{
			this->w = spriteFile.width;
			this->h = spriteFile.height;
			spriteGPU = SpriteGPU::Create(spriteFile,64,64);
			spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
		}

	}Font;

	class FontAsset {
	private:
		std::unordered_map<wchar_t, Font*> fonts;
	public:

		int size = 0;

		FontAsset(const char* ttfPath,int size);
		Font* GetFont(wchar_t c);

		Font* operator [](wchar_t c)
		{
			return GetFont(c);
		}

	};

	/**
	* Canvas有一系列的组组成
	* 而组是有UICamponent
	*/

	//class UICanvas;
	
	class UICanvasComponent;

	class UIComponent
	{
	protected:
		bool active = false;
		friend class UIGroup;
		friend class UICanvasComponent;
	public:
		UITransform transform;
		UITransform fatherTransform;
	protected:

		UIComponent(UIGroup* canvas);
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

	class UIGroup;
	class Canvas 
	{
	protected:
		Canvas();
		virtual ~Canvas();
	public:
		void AddUIGroup(UIGroup* group);

		void OpenGroup(size_t index);
		void OpenGroup(const std::string& id);
		void OpenGroup(UIGroup* group);
		void CloseGroup();

		UIGroup* GetGroupByID(const std::string& id);
		Label* GetLabelByID(const std::string& id);
		Image* GetImageByID(const std::string& id);
		Button* GetButtonByID(const std::string& id);

	protected:
		void CanvasStart();
		void CanvasUpdate();
	private:
		std::stack<UIGroup*> groups;
		std::vector<UIGroup*> groupList;
	};

	class UIGroup final
	{
	private:
		friend class Canvas;
	private:
		UIGroup(Canvas * canvas);
		~UIGroup();
		void Delete(UIGroup*& ptr);
	public:
		static UIGroup* Create(Canvas* canvas);

		UIGroup& SetID(const std::string& id);
		UIGroup& SetPosition(int x,int y);
		UIGroup* Apply();

		Label* GetLabelByID(const std::string& id);
		Image* GetImageByID(const std::string& id);
		Button* GetButtonByID(const std::string& id);

		void AddUIComponent(UIComponent* component);
		size_t GetGroupIndex();
	private:
		void Start();
		void Update();
	private:
		UITransform transform;
		std::vector<UIComponent*> uiComponent;
		size_t index = 0;
	public:
		std::string id = "group";

	};


	class UICanvasActor final : public Actor,public Canvas
	{
	private:
		NOBJECT(UICanvasActor)
	private:
		UICanvasActor(Scene * scene);
		~UICanvasActor() override;
	public:
		static UICanvasActor* Create(Scene* scene);

		void Start() override;
		void Update() override;

	};

	class UICanvasComponent final : public ActorComponent, public Canvas
	{
	private:
		NOBJECT(UICanvasComponent)
	private:
		UICanvasComponent(Actor* actor);
		~UICanvasComponent() override;
	public:
		static UICanvasComponent* Create(Actor* actor);

		void Start() override;
		void Update() override;

	};

	

	class Label :public UIComponent
	{
	public:
		std::string id = "label";
		std::wstring text = L"text";
		uint32_t color = BLACK;
		uint32_t size = 25;

	protected:
		Label(UIGroup* group);
		~Label();

		void Start() override;
		void Update() override;

	public:

		static Label* Create(UIGroup* group);

		Label& SetID(const std::string & id);
		Label& SetColor(uint32_t color);
		Label& SetFontSize(uint32_t size);
		Label& SetPosition(int x,int y);
		Label& SetText(const std::wstring & text);
		Label* Apply();

		

	};
	
	enum class ImageStyle 
	{
		DEFAULT,
		COVER
	};

	class Image :public UIComponent {
	public:
		std::string id = "image";
		ImageStyle style = ImageStyle::DEFAULT;
		uint32_t color = WHITE;
		bool isFilpX = false;
	private:
		Sprite* sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
	private:
		Image(UIGroup* group);
		~Image();

		void Start() override;
		void Update() override;

	public:

		static Image* Create(UIGroup* group);

		Image& SetID(const std::string& id);
		Image& SetPosition(int x,int y);
		Image& SetScale(int x,int y);
		Image& SetStyle(ImageStyle style);
		Image& SetSprite(Sprite * sprite);
		Image& SetColor(uint32_t color);
		Image* Apply();

		

	};

	class Button :public UIComponent
	{
	public:
		std::string id = "button";
		Label* label = nullptr;
		Image* image = nullptr;

		//按键常亮颜色
		uint32_t normalColor = LIGHTGRAY;
		uint32_t selectColor = WHITE;
		uint32_t clickColor = DARKGRAY;
		//按键事件
		NoaEvent<void> clickEvent;

	private:
		bool isClickReady = false;
		bool isSelect = false;
		uint32_t currentColor = normalColor;

	private:
		Button(UIGroup* group);
		~Button();

		void SwapState();

		void Start() override;
		void Update() override;

	public:
		static Button* Create(UIGroup* group);

		
		Button& SetID(const std::string& id);
		Button& SetNormalColor(uint32_t color);
		Button& SetHeightLightColor(uint32_t color);
		Button& SetClickColor(uint32_t color);
		Button& SetPosition(int x,int y);
		Button& SetScale(int w,int h);
		Button& SetSprite(Sprite * sprite);
		Button& SetText(const std::wstring & text);
		Button& SetFontSize(uint32_t size);
		Button& AddClickEvent(std::function<void()> func);
		Button* Apply();
	};
}

#endif // !NOAGUI_H


