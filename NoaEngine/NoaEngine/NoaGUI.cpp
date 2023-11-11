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

	sprite.width = bitmap->width;
	sprite.height = bitmap->rows;
	//sprite.width = size;
	//sprite.height = size;
	sprite.images.resize(sprite.width * sprite.height, ERRORCOLOR);

	if (bitmap->width == 0 || bitmap->rows == 0) {
		sprite.x = 0;
		sprite.y = 0;
		return sprite;
	}

	for (unsigned int y = 0; y < bitmap->rows; y++) {
		for (unsigned int x = 0; x < bitmap->width; x++) {

			uint8_t pixelValue = bitmap->buffer[y * (bitmap->pitch) + x];
			uint32_t pixelColor = (pixelValue >= (256 / 2) ? noa::WHITE : ERRORCOLOR);

			sprite.images[y * sprite.width + x] = pixelColor;

			/*uint8_t pixelValue = bitmap->buffer[y * (bitmap->pitch) + x];
			uint32_t pixelColor = (pixelValue >= (256 / 2)) ? noa::WHITE : ERRORCOLOR;


			int destX = x;
			int destY = size - bitmap->rows + y;

			if (destX < 0 || destY < 0 || destX >= size || destY >= size)
			{
				continue;
			}

			sprite.images[destY * size + destX] = pixelColor;*/

		}
	}

	sprite.x = 0;
	sprite.y = 0;

	return sprite;
}

noa::FontAsset::FontAsset(const char* ttfPath, int size)
{

	this->size = size;

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
		font->bearing.x = face->glyph->bitmap_left;
		font->bearing.y = face->glyph->bitmap_top;
		font->advance = face->glyph->advance.x;
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

noa::Canvas::Canvas() {

}

noa::Canvas::~Canvas() {
	while (!groups.empty()) 
	{
		groups.pop();
	}
	for (auto & group:groupList)
	{
		group->Delete(group);
	}
}

void noa::Canvas::AddUIGroup(UIGroup * group) 
{
	if (group == nullptr) 
	{
		return;
	}
	group->id = groupList.size();
	this->groupList.push_back(group);
	if (groups.empty()) 
	{
		OpenGroup(group->id);
	}

}

void noa::Canvas::OpenGroup(size_t id) 
{
	if (id>=groupList.size()
		||groupList[id] == nullptr)
	{
		return;
	}
	this->groups.push(groupList[id]);
}

void noa::Canvas::CloseGroup() {
	if (groups.size()<=1) 
	{
		return;
	}
	groups.pop();
}

void noa::Canvas::CanvasStart() {
	
}

void noa::Canvas::CanvasUpdate() 
{
	if (groups.empty())
	{
		return;
	}
	if (groups.top() == nullptr) 
	{
		groups.pop();
	}
	else {
		groups.top()->Update();
	}
}

noa::UIGroup::UIGroup(Canvas * canvas) 
{
	if (canvas)
	{
		canvas->AddUIGroup(this);
	}
}

noa::UIGroup::~UIGroup() {
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

noa::UIGroup* noa::UIGroup::Create(noa::Canvas* canvas) 
{
	if (canvas == nullptr) 
	{
		return nullptr;
	}

	UIGroup* group = new UIGroup(canvas);
	return group;

}

void noa::UIGroup::Delete(UIGroup *& ptr) 
{
	delete this;
	ptr = nullptr;
}

void noa::UIGroup::AddUIComponent(UIComponent* component) 
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActive(true);
	uiComponent.push_back(component);
}

size_t noa::UIGroup::GetGroupID() {
	return this->id;
}

void noa::UIGroup::Start() {
	for (auto& component : uiComponent)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}
}

void noa::UIGroup::Update() 
{
	//显示背景
	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->GetActive())
		{
			continue;
		}
		component->Update();
	}
}

noa::UICanvasActor::UICanvasActor(Scene* scene) :Actor(scene), Canvas()
{

}

noa::UICanvasActor::~UICanvasActor()
{
	
}

noa::UICanvasActor* noa::UICanvasActor::Create(Scene* scene)
{
	return noa::NObject<UICanvasActor>::Create(scene);
}

void noa::UICanvasActor::Start()
{
	Canvas::CanvasStart();
}

void noa::UICanvasActor::Update()
{
	Canvas::CanvasUpdate();
}

noa::UICanvasComponent::UICanvasComponent(Actor* actor) :ActorComponent(actor)
{

}

noa::UICanvasComponent::~UICanvasComponent() {
	
}

noa::UICanvasComponent* noa::UICanvasComponent::Create(Actor * actor) 
{
	return noa::NObject<UICanvasComponent>::Create(actor);
}

void noa::UICanvasComponent::Start() {
	Canvas::CanvasStart();
}

void noa::UICanvasComponent::Update() {
	Canvas::CanvasUpdate();
}


noa::Button::Button(UIGroup* group) :UIComponent(group)
{
	image = Image::Create(group);
	label = Label::Create(group);

	this->transform.scale = { 320,160 };

}

noa::Button::~Button()
{

}

noa::Button* noa::Button::Create(UIGroup* group)
{
	Button* button = new Button(group);
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
			/*+ 0.5f * transform.scale.x
			- label->text.length() * 0.5f * label->size*/);
	label->transform.position.y =
		static_cast<int>(
			transform.position.y
			/*+ 0.5 * transform.scale.y
			- 0.5 * label->size*/);
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

noa::UIComponent::UIComponent(noa::UIGroup* group)
{
	if (group)
	{
		group->AddUIComponent(this);
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

noa::Label::Label(UIGroup* group) :UIComponent(group)
{

}

noa::Label::~Label()
{

}

noa::Label* noa::Label::Create(UIGroup* group)
{
	Label* text = new Label(group);
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
	renderer->DrawString(
		text
		, transform.position.x
		, transform.position.y
		, color
		, size
	);

}



noa::Image::Image(UIGroup* group) :UIComponent(group)
{

}

noa::Image::~Image()
{
	
}

noa::Image* noa::Image::Create(UIGroup* group)
{
	Image* image = new Image(group);
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


