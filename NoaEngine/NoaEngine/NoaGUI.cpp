#include "NoaEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace noa {
	extern std::shared_ptr<Renderer> renderer;

	extern int pixelWidth;
	extern int pixelHeight;

	SpriteFile CreateSpriteFromBitmap(FT_Bitmap * bitmap,int size) 
	{
		SpriteFile sprite;

		sprite.width = size;
		sprite.height = size;
		sprite.images.resize(size * size, ERRORCOLOR);

		if (bitmap->width == 0 || bitmap->rows == 0) {
			sprite.x = 0;
			sprite.y = 0;
			return sprite;
		}

		for (unsigned int y = 0; y < bitmap->rows; y++) {
			for (unsigned int x = 0; x < bitmap->width; x++) {

				uint8_t pixelValue = bitmap->buffer[y * (bitmap->pitch) + x];
				uint32_t pixelColor = (pixelValue>=(256/2))?WHITE:ERRORCOLOR;


				int destX = x;
				int destY = size - bitmap->rows + y;

				if (destX<0||destY<0||destX >= size ||destY >= size)
				{
					continue;
				}

				sprite.images[destY * size + destX] = pixelColor;

			}
		}

		sprite.x = 0;
		sprite.y = 0;

		return sprite;
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

			Font* font = new Font(CreateSpriteFromBitmap(&face->glyph->bitmap,size));
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

	UICanvas::UICanvas(Scene* scene) :Actor(scene)
	{

	}

	UICanvas::~UICanvas()
	{
		if (!this->uiComponent.empty())
		{
			auto last = std::unique(uiComponent.begin(), uiComponent.end());
			uiComponent.erase(last, uiComponent.end());

			for (auto& component : uiComponent)
			{
				if (component)
				{
					component->Delete(component);
				}
			}

		}
	}

	UICanvas* UICanvas::Create(Scene* scene)
	{
		return new UICanvas(scene);
	}

	void UICanvas::AddUIComponent(UIComponent* component)
	{
		if (component == nullptr)
		{
			return;
		}
		component->SetActive(true);
		uiComponent.push_back(component);
	}

	void UICanvas::SetActive(bool active)
	{
		Actor::SetActive(active);
		for (int i = 0; i < uiComponent.size(); i++)
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
		for (int i = 0; i < uiComponentCount; i++)
		{
			if (!uiComponent[i]->GetActive())
			{
				continue;
			}
			uiComponent[i]->Update();

		}
	}

	Button::Button(UICanvas* canvas):UIComponent()
	{
		if (canvas) 
		{
			canvas->AddUIComponent(this);
		}
		image = Image::Create(canvas);
		text = Text::Create(canvas);

	}

	Button::~Button()
	{

	}

	Button* Button::Create(UICanvas* canvas)
	{
		Button* button = new Button(canvas);
		return button;
	}


	void Button::SwapState()
	{

		//下面函数在全屏时会出现bug，需要解决

		if (!isActive)
		{
			return;
		}

		const Vector<double> & mousePos = inputSystem.GetMousePosition();

		const float posX = (static_cast<float>(mousePos.x));
		const float posY = (static_cast<float>(mousePos.y));

		if (posX >transform.position.x&& posX<transform.position.x+transform.scale.x
			&& posY>transform.position.y && posY < transform.position.y + transform.scale.y
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

	void Button::Start()
	{

	}

	void Button::Update()
	{

		this->SwapState();

		image->transform = transform;
		image->color = currentColor;


		text->transform.position.x = transform.position.x + 0.5 * transform.scale.x - text->text.length() * 0.5 * text->size * text->narrow;
		text->transform.position.y = transform.position.y + 0.5 * transform.scale.y -  0.5 * text->size * text->narrow;
	}

	void Button::AddClickEvent(std::function<void()> func)
	{
		this->clickEvent += func;
	}

	UIComponent::UIComponent()
	{

	}

	UIComponent::~UIComponent()
	{
		
	}

	void UIComponent::Delete(UIComponent*& ptr)
	{
		delete this;
		ptr = nullptr;
	}

	void UIComponent::SetActive(bool active)
	{
		this->isActive = active;
	}

	bool UIComponent::GetActive()
	{
		return isActive;
	}

	Text::Text(UICanvas* canvas):UIComponent()
	{
		if (canvas) 
		{
			canvas->AddUIComponent(this);
		}
	}

	Text::~Text()
	{

	}

	Text* Text::Create(UICanvas* canvas)
	{
		Text* text = new Text(canvas);
		return text;
	}

	void Text::Start()
	{

	}

	void Text::Update()
	{
		//显示文字
		renderer->DrawString(text, transform.position.x, transform.position.y,narrow, color, size);
	}

	

	Image::Image(UICanvas* canvas):UIComponent()
	{
		if (canvas)
		{
			canvas->AddUIComponent(this);
		}
	}

	Image::~Image()
	{
		if (spriteGPU)
		{
			delete spriteGPU;
			spriteGPU = nullptr;
		}
	}

	Image* Image::Create(UICanvas* canvas)
	{
		Image* image = new Image(canvas);
		return image;
	}

	void Image::SetSprite(Sprite* sprite)
	{
		if (!sprite)
		{
			return;
		}
		this->sprite = sprite;
		if (spriteGPU)
		{
			spriteGPU->Update(sprite);
			spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
			return;
		}
		spriteGPU = new SpriteGPU(sprite);
		spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
	}


	void Image::Start()
	{

	}

	void Image::Update()
	{

		if (sprite == nullptr)
		{
			renderer->DrawRect(transform.position, transform.position + transform.scale, color);
		}
		else {
			spriteGPU->DrawSprite(
				transform.position.x
				,transform.position.y
				,transform.scale.x
				,transform.scale.y
				,color
				,false
				,0.0f
			);
		}



	}

}

