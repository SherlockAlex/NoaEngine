#include "NoaEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace noa {
	extern std::shared_ptr<Renderer> renderer;

	extern int pixelWidth;
	extern int pixelHeight;

	SpriteFile CreateSpriteFromBitmap(FT_Bitmap * bitmap) 
	{
		SpriteFile sprite;

		// 固定大小为 48x48
		sprite.width = 48;
		sprite.height = 48;

		// 初始化 images 字段，将所有像素置为黑色
		sprite.images.resize(48 * 48, ERRORCOLOR);

		// 如果位图为空，则返回一个空的SpriteFile
		if (bitmap->width == 0 || bitmap->rows == 0) {
			sprite.x = 0;
			sprite.y = 0;
			return sprite;
		}

		// 将位图数据拷贝到SpriteFile的images字段中
		for (unsigned int y = 0; y < bitmap->rows; y++) {
			for (unsigned int x = 0; x < bitmap->width; x++) {
				// 在这里根据位图的格式将数据复制到images字段中
				// 由于不清楚位图的格式，这里假设位图的每个像素都是单通道的灰度值（8位或更多）
				// 你可能需要根据实际的位图格式进行适当的处理
				
				//uint8_t pixelValue = bitmap_mono_get_pixel(bitmap->buffer,bitmap->width,bitmap->rows,x,y);
				uint8_t pixelValue = bitmap->buffer[y * (bitmap->pitch) + x];
				uint32_t pixelColor = (pixelValue>=(256/2))?WHITE:ERRORCOLOR;


				// 将像素放置到画布的左下方
				int destX = x;
				int destY = 48 - bitmap->rows + y;

				if (destX<0||destY<0||destX>=48||destY>=48)
				{
					continue;
				}

				// 将像素颜色合并
				sprite.images[destY * 48 + destX] = pixelColor;

			}
		}

		// 设置位图的位置（这部分根据实际需求设置）
		sprite.x = 0;
		sprite.y = 0;

		return sprite;
	}

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
						pixelValue = ERRORCOLOR;
					}
					pixelData.sprites.images.push_back(pixelValue);
				}

				resultData.push_back(pixelData);
			}

			inputFile.close();
		}
		else {
			Debug::Error("Can't find the font assets file");
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
	}

	FontAsset::FontAsset(const char* ttfPath, int size)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			Debug::Error("Init FreeType failed");
			exit(-1);
		}

		FT_Face face;
		if (FT_New_Face(ft, ttfPath, 0, &face)) {
			Debug::Error("Init FT_Face failed");
			exit(-1);
		}

		FT_Set_Pixel_Sizes(face, 0, size);

		for (unsigned char c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				continue;
			}

			Font* font = new Font(CreateSpriteFromBitmap(&face->glyph->bitmap));
			this->fonts[c] = font;
		}

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

		Vector<double> mousePos = inputSystem.GetMousePosition();

		if (mousePos.x>transform.position.x&&mousePos.x<transform.position.x+scale.x
			&& mousePos.y>transform.position.y && mousePos.y < transform.position.y + scale.y
			) 
		{
			isSelect = true;

			if (inputSystem.GetMouseButton(MouseButton::LEFT_BUTTON))
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

		this->SwapState();

		if (sprite == nullptr)
		{
			renderer->DrawRect(transform.position, transform.position + scale, currentColor);
		}
		else {
			renderer->DrawRect(transform.position, transform.position + scale, sprite,currentColor,true);
		}
		
		//显示文字
		renderer->DrawString(text, 
			static_cast<int>(transform.position.x + 0.5 * scale.x - text.length() * 0.5 * fontSize * fontNarrowX)
			, static_cast<int>(transform.position.y + 0.5 * scale.y - 0.5 * fontSize * fontNarrowX)
			, fontNarrowX
			, textColor
			, fontSize);
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
		renderer->DrawString(text, transform.position.x, transform.position.y, textColor, size);
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
			}
		}
	}

	UICanvas* UICanvas::Create(Scene* scene)
	{
		return new UICanvas(scene);
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
		for (int i = 0; i < static_cast<int>(uiComponent.size()); i++)
		{
			uiComponent[i]->Start();
		}
	}

	void UICanvas::Update()
	{
		const int uiComponentCount = static_cast<int>(uiComponent.size());
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
			renderer->DrawRect(transform.position, transform.position + scale, color);
		}
		else 
		{
			renderer->DrawRect(transform.position, transform.position + scale, sprite, color, true);
		}
		
	}

}

