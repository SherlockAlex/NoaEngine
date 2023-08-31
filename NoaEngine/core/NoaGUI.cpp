#include "NoaGUI.h"
#include <unordered_map>
#include "Scene.h"
#include "Renderer.h"
#include "InputSystem.h"

namespace noa {
	extern void Debug(string msg);
	extern Renderer renderer;

	FontAsset::FontAsset(const char * fontPath)
	{
		//������Ϸ��������Դ

		unordered_map<int, Font*> result;
		std::vector<PixelData> resultData;

		std::ifstream inputFile(fontPath, std::ios::binary);
		if (inputFile)
		{
			int tileCount;
			inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

			Debug("font asset size:" + to_string(tileCount));

			for (int i = 0; i < tileCount; i++)
			{
				PixelData pixelData;
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
					pixelData.sprites.images.push_back(pixelValue);
				}

				resultData.push_back(pixelData);
			}

			inputFile.close();
		}
		else {
			Debug("can't find the font assets file");
			exit(0);
		}

		for (PixelData data : resultData)
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
		UIComponent::~UIComponent();
	}

	void NoaButton::SwapState()
	{
		//������ķ�Χ������ڷ�Χ�ھ��л�״̬
		if (!isActive)
		{
			return;
		}

		Vector<float> mousePos = inputSystem.GetMousePosition();

		if (mousePos.x>position.x&&mousePos.x<position.x+scale.x
			&& mousePos.y>position.y && mousePos.y < position.y + scale.y
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
			renderer.DrawRect(position, position + scale, currentColor);
		}
		else {
			renderer.DrawRect(position, position + scale, sprite,currentColor,true);
		}
		
		//��ʾ����
		renderer.DrawString(text, position.x + 10, position.y + 10, textColor, 20);
	}

	void NoaButton::AddClickEvent(function<void()> func)
	{
		this->clickEvent += func;
	}

	UIComponent::UIComponent():Behaviour()
	{
	}

	UIComponent::~UIComponent()
	{
		Behaviour::~Behaviour();
	}

	NoaText::NoaText():UIComponent()
	{

	}

	NoaText::~NoaText()
	{
		UIComponent::~UIComponent();
	}

	void NoaText::Start()
	{

	}

	void NoaText::Update()
	{
		//��ʾ����
		renderer.DrawString(text, position.x, position.y, textColor, size);
	}

	NoaCanvase::NoaCanvase()
	{
	}

	NoaCanvase::~NoaCanvase()
	{
	}

	void NoaCanvase::SetActive(bool active)
	{
		for (int i=0;i<uiComponent.size();i++) 
		{
			uiComponent[i]->isActive = active;
		}
	}

}

