#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include "Renderer.h"
#include "Debug.h"
#include "NoaGUI.h"
#include "Sprite.h"
#include "InputSystem.h"
#include "Screen.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

}

noa::SpriteFile CreateSpriteFromBitmap(FT_Bitmap* bitmap, int size)
{
	noa::SpriteFile sprite;

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
			uint32_t pixelColor = (pixelValue >= (256 / 2)) ? noa::WHITE : ERRORCOLOR;


			int destX = x;
			int destY = size - bitmap->rows + y;

			if (destX < 0 || destY < 0 || destX >= size || destY >= size)
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

noa::FontAsset::FontAsset(const char* ttfPath, int size)
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

		Font* font = new Font(CreateSpriteFromBitmap(&face->glyph->bitmap, size));
		this->fonts[c] = font;
	}

}

noa::Font* noa::FontAsset::GetFont(char c)
{
	if (this->fonts.count(c)<=0) 
	{
		return nullptr;
	}
	return fonts[c];
}

noa::UICanvas::UICanvas(Scene* scene) :Actor(scene)
{

}

noa::UICanvas::~UICanvas()
{
	if (!this->uiComponent.empty())
	{
		//对component进行排序
		std::sort(uiComponent.begin(), uiComponent.end());
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

noa::UICanvas* noa::UICanvas::Create(Scene* scene)
{
	return noa::NObject<UICanvas>::Create(scene);
}

void noa::UICanvas::AddUIComponent(UIComponent* component)
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActive(true);
	uiComponent.push_back(component);
}

void noa::UICanvas::SetActive(bool active)
{
	Actor::SetActive(active);

	for (auto & component:uiComponent) 
	{
		if (component!=nullptr) 
		{
			component->SetActive(active);
		}

	}
}

void noa::UICanvas::Start()
{

	for (auto & component:uiComponent) 
	{
		if (component!=nullptr) 
		{
			component->Start();
		}
	}

}

void noa::UICanvas::Update()
{
	//显示背景
	for (auto& component : uiComponent)
	{
		if (component == nullptr||!component->GetActive())
		{
			continue;
		}
		component->Update();
	}
}

noa::UICanvasComponent::UICanvasComponent(Actor* actor) :ActorComponent(actor)
{

}

noa::UICanvasComponent::~UICanvasComponent() {
	if (!uiComponent.empty()) 
	{
		std::sort(uiComponent.begin(),uiComponent.end());
		auto last = std::unique(uiComponent.begin(), uiComponent.end());
		uiComponent.erase(last,uiComponent.end());

		for (auto & component:uiComponent) 
		{
			if (component!=nullptr)
			{
				component->Delete(component);
			}
		}

	}
}

noa::UICanvasComponent* noa::UICanvasComponent::Create(Actor * actor) 
{
	return noa::NObject<UICanvasComponent>::Create(actor);
}

void noa::UICanvasComponent::AddUIComponent(UIComponent* component) 
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActive(true);
	uiComponent.push_back(component);
}

void noa::UICanvasComponent::SetActive(bool active)
{
	ActorComponent::SetActive(active);

	for (auto & component:uiComponent)
	{
		if (component!=nullptr)
		{
			component->SetActive(active);
		}
	}
}

void noa::UICanvasComponent::Start() {

	for (auto & component:uiComponent) 
	{
		if (component!=nullptr) 
		{
			component->Start();
		}
	}

}

void noa::UICanvasComponent::Update() {
	for (auto & component:uiComponent) 
	{
		if (component == nullptr||!component->GetActive()) 
		{
			continue;
		}
		component->Update();
	}
}


noa::Button::Button(UICanvas* canvas) :UIComponent(canvas)
{
	image = Image::Create(canvas);
	label = Label::Create(canvas);

	this->transform.scale = { 320,160 };

}

noa::Button::Button(UICanvasComponent* canvas) :UIComponent(canvas)
{
	image = Image::Create(canvas);
	label = Label::Create(canvas);

	this->transform.scale = { 320,160 };

}

noa::Button::~Button()
{

}

noa::Button* noa::Button::Create(UICanvas* canvas)
{
	Button* button = new Button(canvas);
	return button;
}

noa::Button* noa::Button::Create(UICanvasComponent* canvas)
{
	Button* button = new Button(canvas);
	return button;
}


void noa::Button::SwapState()
{

	//下面函数在全屏时会出现bug，需要解决

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float posX = (static_cast<float>(mousePos.x));
	const float posY = (static_cast<float>(mousePos.y));

	if (posX > transform.position.x && posX<transform.position.x + transform.scale.x
		&& posY>transform.position.y && posY < transform.position.y + transform.scale.y
		)
	{
		isSelect = true;

		if (Input::GetMouseKeyUp(MouseButton::LEFT_BUTTON))
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

void noa::Button::Start()
{

}

void noa::Button::Update()
{

	this->SwapState();

	image->transform = transform;
	image->color = currentColor;


	label->transform.position.x =
		static_cast<int>(
			transform.position.x
			+ 0.5 * transform.scale.x
			- label->text.length() * 0.5 * label->size * label->narrow);
	label->transform.position.y =
		static_cast<int>(
			transform.position.y
			+ 0.5 * transform.scale.y
			- 0.5 * label->size * label->narrow);
}

noa::Button& noa::Button::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Button& noa::Button::SetScale(int w,int h) 
{
	this->transform.scale.x = w;
	this->transform.scale.y = h;
	return *this;
}

noa::Button& noa::Button::SetSprite(noa::Sprite* sprite) {
	this->image->SetSprite(sprite);
	return *this;
}

noa::Button& noa::Button::SetText(const std::string& text)
{
	this->label->text = text;
	return *this;
}

noa::Button& noa::Button::SetFontSize(uint32_t size) 
{
	this->label->SetFontSize(size);
	return *this;
}

noa::Button& noa::Button::AddClickEvent(std::function<void()> func)
{
	this->clickEvent += func;
	return *this;
}

noa::Button* noa::Button::Apply() {
	return this;
}

noa::UIComponent::UIComponent(noa::UICanvas* canvas)
{
	if (canvas)
	{
		canvas->AddUIComponent(this);
	}
}

noa::UIComponent::UIComponent(noa::UICanvasComponent* canvas)
{
	if (canvas) 
	{
		canvas->AddUIComponent(this);
	}
}

noa::UIComponent::~UIComponent()
{

}

void noa::UIComponent::Delete(UIComponent*& ptr)
{
	delete this;
	ptr = nullptr;
}

void noa::UIComponent::SetActive(bool active)
{
	this->active = active;
}

bool noa::UIComponent::GetActive()
{
	return active;
}

noa::Label::Label(UICanvas* canvas) :UIComponent(canvas)
{

}

noa::Label::Label(UICanvasComponent* canvas) :UIComponent(canvas)
{
}

noa::Label::~Label()
{

}

noa::Label* noa::Label::Create(UICanvas* canvas)
{
	Label* text = new Label(canvas);
	return text;
}

noa::Label* noa::Label::Create(UICanvasComponent* canvas)
{
	Label* text = new Label(canvas);
	return text;
}

noa::Label& noa::Label::SetFontSize(uint32_t size)
{
	this->size = size;
	return *this;
}

noa::Label& noa::Label::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Label& noa::Label::SetScale(int w,int h) 
{
	this->transform.scale.x = w;
	this->transform.scale.y = h;
	return *this;
}

noa::Label* noa::Label::Apply() 
{
	return this;
}


void noa::Label::Start()
{

}

void noa::Label::Update()
{
	//显示文字
	renderer->DrawString(text, transform.position.x, transform.position.y, narrow, color, size);
}



noa::Image::Image(UICanvas* canvas) :UIComponent(canvas)
{

}

noa::Image::Image(UICanvasComponent* canvas) :UIComponent(canvas) {

}

noa::Image::~Image()
{
	
}

noa::Image* noa::Image::Create(UICanvas* canvas)
{
	Image* image = new Image(canvas);
	return image;
}

noa::Image* noa::Image::Create(UICanvasComponent* canvas)
{
	Image* image = new Image(canvas);
	return image;
}

noa::Image& noa::Image::SetStyle(noa::ImageStyle style)
{
	this->style = style;
	return *this;
}

noa::Image & noa::Image::SetSprite(Sprite* sprite)
{
	if (!sprite)
	{
		return *this;
	}
	this->sprite = sprite;
	if (spriteGPU)
	{
		spriteGPU->Update(sprite);
		spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
		return *this;
	}
	spriteGPU = SpriteGPU::Create(sprite);
	spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
	return *this;
}

noa::Image* noa::Image::Apply() {
	return this;
}

void noa::Image::Start()
{

}

void noa::Image::Update()
{

	if (sprite == nullptr)
	{
		return;
	}
	
	switch (style)
	{
	case noa::ImageStyle::COVER:
		spriteGPU->DrawSprite(
			0
			, 0
			, static_cast<float>(noa::Screen::width)
			, static_cast<float>(noa::Screen::height)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	default:
		spriteGPU->DrawSprite(
			static_cast<float>(transform.position.x)
			, static_cast<float>(transform.position.y)
			, static_cast<float>(transform.scale.x)
			, static_cast<float>(transform.scale.y)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	}

	

}


