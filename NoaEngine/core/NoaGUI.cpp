#include "NoaGUI.h"
#include <unordered_map>
#include "Scene.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Resource.h"

namespace noa {
	extern void Debug(string msg);
	extern Renderer renderer;

	extern int pixelWidth;
	extern int pixelHeight;

	FontAsset::FontAsset(const char * fontPath)
	{
		//加载游戏的字体资源

		std::unordered_map<int, Font*> result;
		std::vector<TileData> resultData;

		std::ifstream inputFile(fontPath, std::ios::binary);
		if (inputFile)
		{
			int tileCount;
			inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

			Debug("Font asset size:" + to_string(tileCount));

			for (int i = 0; i < tileCount; i++)
			{
				TileData pixelData;
				inputFile.read(reinterpret_cast<char*>(&pixelData.id), sizeof(int));

				int imageCount;
				inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.x), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.y), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.width), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.height), sizeof(int));

				for (int j = 0; j < imageCount; j++)
				{
					int pixelValue;
					inputFile.read(reinterpret_cast<char*>(&pixelValue), sizeof(int));
					if (pixelValue == 0) 
					{
						//将黑色调为透明色
						pixelValue = ERRORCOLOR;
					}
					pixelData.sprites.images.push_back(pixelValue);
				}

				resultData.push_back(pixelData);
			}

			inputFile.close();
		}
		else {
			Debug("Can't find the font assets file");
			exit(-1);
		}

		for (TileData data : resultData)
		{
			Font* font = new Font(data.sprites);
			result[data.id] = font;
		}

		

		for (int i = 0; i < 9*11-4; i++)
		{
			char c = (char)(i + 32);
			this->fonts[c] = result[i];
			
		}
		Debug("Init font asset successfully");
		//this->fonts = result;
		//return result;
	}

	Font* FontAsset::GetFont(char c)
	{
		if (!ContainKey<char,Font*>(this->fonts,c)) 
		{
			return nullptr;
		}
		return fonts[c];
	}

	NoaButton::NoaButton():UIComponent()
	{
		
		inputSystem.inputEvent += [this](void) {SwapState(); };
	}

	NoaButton::~NoaButton()
	{

	}

	NoaButton* NoaButton::Create()
	{
		return new NoaButton();
	}

	void NoaButton::Delete()
	{
		delete this;
	}

	void NoaButton::SwapState()
	{
		//检测鼠标的范围，如果在范围内就切换状态
		if (!isActive)
		{
			return;
		}

		Vector<float> mousePos = inputSystem.GetMousePosition();

		if (mousePos.x>transform.position.x&&mousePos.x<transform.position.x+scale.x
			&& mousePos.y>transform.position.y && mousePos.y < transform.position.y + scale.y
			) 
		{
			isSelect = true;

			if (inputSystem.GetMouseButton(LeftButton))
			{
				this->clickEvent.Invoke();
			}

		}
		else {
			isSelect = false;
		}

		if (isSelect)
		{
			currentColor = selectColor;
		}
		else {
			currentColor = normalColor;
		}

		

	}

	void NoaButton::Start()
	{

	}

	void NoaButton::Update()
	{
		if (sprite == nullptr)
		{
			renderer.DrawRect(transform.position, transform.position + scale, currentColor);
		}
		else {
			renderer.DrawRect(transform.position, transform.position + scale, sprite,currentColor,true);
		}
		
		
		//显示文字
		renderer.DrawString(text, transform.position.x + 0.5*scale.x - text.length() * 0.5*fontSize* fontNarrowX, transform.position.y + 0.5*scale.y - 0.5*fontSize* fontNarrowX, fontNarrowX, textColor, fontSize);
	}

	void NoaButton::AddClickEvent(std::function<void()> func)
	{
		this->clickEvent += func;
	}

	UIComponent::UIComponent()
	{

	}

	UIComponent::~UIComponent()
	{
		
	}

	void UIComponent::SetActive(bool active)
	{
		this->isActive = active;
	}

	bool UIComponent::GetActive()
	{
		return isActive;
	}

	NoaText::NoaText():UIComponent()
	{

	}

	NoaText::~NoaText()
	{

	}

	NoaText* NoaText::Create()
	{
		return new NoaText();
	}

	void NoaText::Delete()
	{
		delete this;
	}

	void NoaText::Start()
	{

	}

	void NoaText::Update()
	{
		//显示文字
		renderer.DrawString(text, transform.position.x, transform.position.y, textColor, size);
	}

	UICanvas::UICanvas(Scene* scene):Actor(scene)
	{

	}

	UICanvas::~UICanvas()
	{
		if (!this->uiComponent.empty())
		{
			auto last = std::unique(uiComponent.begin(), uiComponent.end());
			uiComponent.erase(last, uiComponent.end());
			for (int i = 0;i<uiComponent.size();i++) 
			{
				if (uiComponent[i]==nullptr)
				{
					continue;
				}
				uiComponent[i]->Delete();
				Debug("Remove UI component");
			}
		}
	}

	UICanvas* UICanvas::Create(Scene* scene)
	{
		return new UICanvas(scene);
	}

	void UICanvas::Delete()
	{
		delete this;
	}

	void UICanvas::AddComponent(UIComponent* component)
	{
		if (component==nullptr)
		{
			return;
		}
		component->SetActive(true);
		uiComponent.push_back(component);
	}

	void UICanvas::SetActive(bool active)
	{
		Actor::SetActive(active);
		for (int i=0;i<uiComponent.size();i++) 
		{
			uiComponent[i]->SetActive(active);
		}
	}

	void UICanvas::Start()
	{
		for (int i = 0; i < uiComponent.size(); i++)
		{
			uiComponent[i]->Start();
		}
	}

	void UICanvas::Update()
	{
		const int uiComponentCount = uiComponent.size();
		for (int i=0;i< uiComponentCount;i++)
		{
			if (!uiComponent[i]->GetActive())
			{
				continue;
			}
			uiComponent[i]->Update();
			
		}
	}

	NoaImage::NoaImage():UIComponent()
	{

	}

	NoaImage::~NoaImage()
	{

	}

	NoaImage* NoaImage::Create()
	{
		return new NoaImage();
	}

	void NoaImage::Delete()
	{
		delete this;
	}

	void NoaImage::Start()
	{

	}

	void NoaImage::Update()
	{
		if (sprite==nullptr) 
		{
			renderer.DrawRect(transform.position, transform.position + scale, color);
		}
		else 
		{
			renderer.DrawRect(transform.position, transform.position + scale, sprite, color, true);
		}
		
	}

}

