#include "UIImage.h"
#include "Screen.h"

noa::Image::Image(UIContainer* group) :UIComponent(group)
{
	sprite.w = 320;
	sprite.h = 320;
	sprite.size = { 320,320 };
	sprite.ResizeAndFull(320, 320, noa::RGBA(255, 255, 255, 255));

	spriteGPU = SpriteGPU::Create(&sprite);
	spriteGPU->SetLayer(InstanceLayer::UI_LAYER);

}

noa::Image::~Image()
{

}

noa::Image* noa::Image::Create(UIContainer* group)
{
	Image* image = new Image(group);
	return image;
}

noa::Image& noa::Image::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::Image& noa::Image::SetLocalPosition(int x, int y)
{
	noa::UIBody::SetLocalPosition(x,y);
	return *this;
}

noa::Image& noa::Image::SetGlobalPosition(int x,int y) 
{
	noa::UIBody::SetGlobalPosition(x,y);
	return *this;
}

noa::Image& noa::Image::SetAnchor(float x, float y)
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::Image& noa::Image::SetActive(bool value)
{
	UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::Image& noa::Image::SetLocalSize(int x, int y)
{

	noa::UIBody::SetLocalSize(x, y);

	this->sprite.size.x = x;
	this->sprite.size.y = y;
	this->sprite.ResizeAndFull(x, y, noa::RGBA(255, 255, 255, 255));

	return *this;
}

noa::Image& noa::Image::SetRadius(int value)
{

	this->sprite.Full(noa::RGBA(255, 255, 255, 255));
	if (mapSprite)
	{
		this->sprite.MapFromSprite(*mapSprite);
	}

	if (value <= 0)
	{
		radius = 0;
		return *this;
	}

	const int maxValue = (sprite.w < sprite.h) ? (sprite.w / 2) : (sprite.h / 2);
	radius = value;
	if (radius > maxValue)
	{
		radius = maxValue;
	}

	const int x1 = radius;
	const int x2 = sprite.w - radius;
	const int y1 = radius;
	const int y2 = sprite.h - radius;

	const int sqrRadius = (radius) * (radius);

	//вСио╫г
	for (int x = 0; x < radius; x++)
	{
		for (int y = 0; y < radius; y++)
		{
			const int deltaX = x - x1;
			const int deltaY = y - y1;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//вСоб╫г
	for (int x = 0; x < radius; x++)
	{
		for (int y = y2; y < sprite.h; y++)
		{
			const int deltaX = x - x1;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//сроб╫г
	for (int x = x2; x < sprite.w; x++)
	{
		for (int y = y2; y < sprite.h; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));

			}
		}
	}

	//срио╫г
	for (int x = x2; x < sprite.w; x++)
	{
		for (int y = 0; y < radius; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y1;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius)
			{

				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	return *this;
}

noa::Image& noa::Image::SetStyle(noa::ImageStyle style)
{
	this->style = style;
	return *this;
}

noa::Image& noa::Image::SetSprite(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		Sprite spriteBuffer;
		spriteBuffer.ResizeAndFull(10, 10, noa::RGBA(255, 255, 255, 255));
		this->sprite.MapFromSprite(spriteBuffer);
		return *this;
	}

	this->mapSprite = sprite;
	this->sprite.MapFromSprite(*mapSprite);
	return *this;
}

noa::Image& noa::Image::SetColor(uint32_t color)
{
	this->color = color;
	return *this;
}

noa::Image* noa::Image::Apply()
{
	spriteGPU->Update(&sprite);
	return this;
}

void noa::Image::Start()
{

}

void noa::Image::Update()
{
	noa::UIBody::OnUpdate();
}

void noa::Image::Render() {

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
			globalTransform.position.x 
				- 2*anchor.x * transform.size.x
			, globalTransform.position.y 
				- 2*anchor.y * transform.size.y
			, static_cast<float>(transform.size.x)
			, static_cast<float>(transform.size.y)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	}
}