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
	class UIDocument;
	class UIContainer;
	class UIDocumentComponent;

	enum class ImageStyle
	{
		DEFAULT,
		COVER
	};


	//����
	typedef struct Font
	{
		int w = 0;
		int h = 0;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

		noa::Vector<int> bearing;	//�ӻ�׼�ߵ�������/������ƫ��ֵ
		uint32_t advance = 0;			//ԭ�����һ������ԭ��ľ���

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
		UITransform transform;
		UITransform globalTransform;
		Vector<float> anchor = {0.5f,0.5f};
	protected:
		UITransform fatherTransform;	// ���ڵ�ľ���·��
		UIComponent(UIContainer* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent *& ptr);

	protected:

		virtual void Start() = 0;
		virtual void Update() = 0;		//����UI�Ľ����߼�
		virtual void Render() = 0;		//����UI�Ļ���

	public:

		void SetActive(bool active);
		bool GetActive();
	};

	class UIDocument 
	{
	protected:
		UIDocument();
		virtual ~UIDocument();
	public:
		void AddUIContainer(UIContainer* container);

		void Display(size_t index);
		void Display(const std::string& id);
		void Display(UIContainer* container);
		void Close();

		UIContainer* GetContainerByID(const std::string& id);
		Label* GetLabelByID(const std::string& id);
		Image* GetImageByID(const std::string& id);
		Button* GetButtonByID(const std::string& id);

	protected:
		void UIDocumentStart();
		void UIDocumentUpdate();
	private:
		std::stack<UIContainer*> containerStack;
		std::vector<UIContainer*> containerList;
	};

	class UIContainer final
	{
	private:
		friend class UIDocument;
	private:
		UIContainer(UIDocument* canvas);
		~UIContainer();
		void Delete(UIContainer*& ptr);
	public:
		static UIContainer* Create(UIDocument* canvas);

		UIContainer& SetID(const std::string& id);
		UIContainer& SetPosition(int x, int y);
		UIContainer* Apply();

		Label* GetLabelByID(const std::string& id);
		Image* GetImageByID(const std::string& id);
		Button* GetButtonByID(const std::string& id);

		void AddUIComponent(UIComponent* component);
		size_t GetGroupIndex();
	private:
		void Start();
		void Update();
		void Render();
	private:
		bool visiable = false;
		UITransform transform;
		std::vector<UIComponent*> uiComponent;
		size_t index = 0;
	public:
		std::string id = "group";

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
		Label& SetFontSize(uint32_t size);
		Label& SetPosition(int x,int y);
		Label& SetText(const std::wstring & text);
		Label* Apply();

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
		Image& SetSize(int x,int y);
		Image& SetStyle(ImageStyle style);
		Image& SetSprite(Sprite * sprite);
		Image& SetColor(uint32_t color);
		Image* Apply();

		

	};

	class Button :public UIComponent
	{
	public:
		std::string id = "button";

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

		Sprite sprite;

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

		// ���ư�ť���ԣ�����������ť���¼���id��λ��
		Button& Clone(Button* button);
		Button& SetID(const std::string& id);
		Button& SetNormalColor(uint32_t color);
		Button& SetHeightLightColor(uint32_t color);
		Button& SetClickColor(uint32_t color);
		Button& SetPosition(int x,int y);
		Button& SetAnchor(float x,float y);
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
		Button& AddSelectedCallback(std::function<void()> func);
		Button& AddClickCallback(std::function<void()> func);
		Button* Apply();
	};
}

#endif // !NOAGUI_H


