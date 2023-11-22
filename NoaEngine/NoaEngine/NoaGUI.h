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

	class AudioClip;

	class Button;
	class Label;
	class Image;
	class ProcessBar;
	class UIHub;
	class UIDocument;
	class UIContainer;
	class UIDocumentComponent;

	enum class ImageStyle
	{
		DEFAULT,
		COVER
	};


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

	class UIComponent
	{
	protected:
		bool active = false;
		friend class UIContainer;
		friend class UIDocumentComponent;



	public:
		std::string id = "ui_component";
		UITransform transform;
		UITransform globalTransform;
		Vector<float> anchor = { 0.5f,0.5f };
	protected:
		UITransform fatherTransform;	// 父节点的绝对路径
		UIComponent(UIContainer* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent*& ptr);

	protected:

		virtual void Start() = 0;
		virtual void Update() = 0;		//负责UI的交互逻辑
		virtual void Render() = 0;		//负责UI的绘制

	public:

		void SetActiveInContainer(bool active);
		bool GetActive();
	};

	//UIDocument的查找是一个大的问题
	//需要提供一套解决方案

	class UIHub 
	{
	public:
		static UIDocument* GetDocumentByID(const std::string& id);
	private:
		static void RemoveDocument(UIDocument* document);
	private:
		friend class UIDocument;
		static std::vector<UIDocument*> documents;
	};

	class UIDocument 
	{
	protected:
		UIDocument();
		virtual ~UIDocument();
	public:

		void SetDocumentID(const std::string& id);

		void Display(size_t index);
		void Display(const std::string& id);
		void Display(UIContainer* container);
		void Close();

		template<class T>
		T* GetElementByID(const std::string& id) 
		{
			for (auto& container : containerList)
			{
				if (!container)
				{
					continue;
				}

				T* temp = container->GetElementByID<T>(id);
				if (temp)
				{
					return temp;
				}
			}
			return nullptr;
		}

	private:
		void AddUIContainer(UIContainer* container);
	protected:
		void UIDocumentStart();
		void UIDocumentUpdate();
		void UIDocumentRender();
	private:
		friend class UIContainer;
		friend class UIHub;

		std::stack<UIContainer*> containerStack;
		std::vector<UIContainer*> containerList;

		std::string id = "document";

	};

	class UIContainer final
	{
	private:
		friend class UIDocument;
		friend class UIContainer;
		friend class UIComponent;
	private:
		UIContainer(UIDocument* canvas);
		UIContainer(UIContainer* father);
		~UIContainer();
		void Delete(UIContainer*& ptr);
	public:
		static UIContainer* Create(UIDocument* canvas);
		static UIContainer* Create(UIContainer* father);

		UIContainer& SetID(const std::string& id);
		UIContainer& SetPosition(int x, int y);
		UIContainer& SetVisiable(bool value);
		UIContainer* Apply();

		template<class T>
		T* GetElementByID(const std::string& id)
		{

			for (auto& element : uiComponent)
			{
				T* buffer = dynamic_cast<T*>(element);
				if (buffer == nullptr || buffer->id != id)
				{
					continue;
				}
				return buffer;
			}
			return nullptr;
		}

		template<>
		noa::UIContainer* GetElementByID(const std::string& id)
		{

			if (this->id == id)
			{
				return this;
			}

			for (auto& element : subContainers)
			{
				if (element == nullptr || element->id != id)
				{
					continue;
				}
				return element;
			}
			return nullptr;
		}

		
		size_t GetContainerIndex();
	private:
		void Start();
		void Update();
		void Render();

		void DestroyUIContainer();
		void AddUIComponent(UIComponent* component);
		void AddUIContainer(UIContainer* container);
	private:
		bool visiable = false;
		UITransform transform;
		UITransform fatherTransform;
		std::vector<UIComponent*> uiComponent;
		std::vector<UIContainer*> subContainers;
		size_t index = 0;
	public:
		UITransform globalTransform;
		std::string id = "container";
	};


	class UIDocumentActor final : public Actor,public UIDocument
	{
	private:
		NOBJECT(UIDocumentActor)
	private:
		UIDocumentActor(Scene * scene);
		~UIDocumentActor() override;
	public:
		static UIDocumentActor* Create(Scene* scene);

		void Start() override;
		void Update() override;
		void Render() override;
		UIDocumentActor& SetActorTag(const std::string& tag);
		UIDocumentActor& SetID(const std::string& id);
		UIDocumentActor* Apply();

	};

	class UIDocumentComponent final : public ActorComponent, public UIDocument
	{
	private:
		NOBJECT(UIDocumentComponent)
	private:
		UIDocumentComponent(Actor* actor);
		~UIDocumentComponent() override;
	public:
		static UIDocumentComponent* Create(Actor* actor);

		UIDocumentComponent& SetID(const std::string& id);
		UIDocumentComponent* Apply();

		void Start() override;
		void Update() override;
		void Render() override;

	};

	/**
	* UI控件
	* 按钮
	* 图片
	* 文字
	* 进度条
	*/

	class Label :public UIComponent
	{
	public:
		std::wstring text = L"text";
		uint32_t color = BLACK;
		uint32_t size = 25;

	protected:
		Label(UIContainer* group);
		~Label();

		void Start() override;
		void Update() override;
		void Render() override;

	public:

		static Label* Create(UIContainer* group);

		Label& SetID(const std::string & id);
		Label& SetAnchor(float x,float y);
		Label& SetColor(uint32_t color);
		Label& SetActive(bool value);
		Label& SetFontSize(uint32_t size);
		Label& SetPosition(int x,int y);
		Label& SetText(const std::wstring & text);
		Label* Apply();

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
		Image(UIContainer* group);
		~Image();

		void Start() override;
		void Update() override;
		void Render() override;

	public:

		static Image* Create(UIContainer* group);

		Image& SetID(const std::string& id);
		Image& SetPosition(int x,int y);
		Image& SetAnchor(float x,float y);
		Image& SetActive(bool value);
		Image& SetSize(int x,int y);
		Image& SetStyle(ImageStyle style);
		Image& SetSprite(Sprite * sprite);
		Image& SetColor(uint32_t color);
		Image* Apply();

		

	};

	class Button :public UIComponent
	{
	public:

		//按键背景颜色
		uint32_t normalColor = LIGHTGRAY;
		uint32_t heightLightColor = WHITE;
		uint32_t clickColor = DARKGRAY;

		//按键字体颜色
		uint32_t textNormalColor = BLACK;
		uint32_t textHeightLightColor = BLACK;
		uint32_t textClickColor = BLACK;

		//设置按钮大小
		float normalScale = 1.0f;
		float heightLightScale = 1.0f;
		float clickScale = 1.0f;
		
	private:
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

		//按键事件
		NoaEvent<void> selectedEvent;
		NoaEvent<void> clickEvent;

		Sprite sprite;
		
		std::shared_ptr<noa::AudioClip> selectedAudio = nullptr;
		std::shared_ptr<noa::AudioClip> clickAudio = nullptr;


		friend class Button;

	private:
		Button(UIContainer* group);
		~Button();

		void SwapState();

		void Start() override;
		void Update() override;
		void Render() override;

	public:
		static Button* Create(UIContainer* group);

		// 复制按钮属性，但不包括按钮的事件、id、位置
		Button& Clone(Button* button);
		Button& SetID(const std::string& id);
		Button& SetNormalColor(uint32_t color);
		Button& SetHeightLightColor(uint32_t color);
		Button& SetClickColor(uint32_t color);
		Button& SetPosition(int x,int y);
		Button& SetAnchor(float x,float y);
		Button& SetActive(bool value);
		Button& SetSize(int w,int h);
		Button& SetRadius(int value);
		Button& SetTextOffset(float x,float y);
		Button& SetText(const std::wstring & text);
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
	};

	//进度条
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
		ProcessBar& SetPosition(int x,int y);
		ProcessBar& SetAnchor(float x,float y);
		ProcessBar& SetActive(bool value);
		ProcessBar& SetBackgroundColor(uint32_t color);
		ProcessBar& SetFillColor(uint32_t color);
		ProcessBar& SetFinishedCallback(std::function<void()> func);
		ProcessBar& SetSize(int x,int y);
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

		Sprite backgroundSprite;
		Sprite runtimeSprite;

		bool interactable = true;

		NoaEvent<void> finishedEvent;
		float amount = 0.0f;
		float oldAmount = amount;
		int radius = 0;
		bool finished = false;
	};
}

#endif // !NOAGUI_H


