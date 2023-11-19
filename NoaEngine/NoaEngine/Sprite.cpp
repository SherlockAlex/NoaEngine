#include "Sprite.h"
#include "Texture.h"
#include "Renderer.h"
#include "Graphic.h"
#include "Screen.h"

#include "Debug.h"

using namespace std;

namespace noa {

	extern shared_ptr<Renderer> renderer;

}

noa::Sprite::Sprite(const SpriteFile& sprFile, const Vector<int>& scale)
{
	this->posx = sprFile.x;
	this->posx = sprFile.y;
	this->w = sprFile.width;
	this->h = sprFile.height;
	this->image = sprFile.images;
	this->size = scale;

	this->isEmpty = this->image.empty();

	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;
}

noa::Sprite::Sprite(const Vector<int>& size)
{
	this->size = size;
	this->isEmpty = this->image.empty();
}

noa::Sprite::Sprite(const std::string& filename, const Vector<int>& size) {
	SpriteFile sprFile = Resource::LoadSprite(filename);
	this->posx = sprFile.x;
	this->posy = sprFile.y;
	this->w = sprFile.width;
	this->h = sprFile.height;
	this->image = sprFile.images;
	this->size = size;
	this->isEmpty = this->image.empty();

	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;

}

noa::Sprite::Sprite(const std::string& filename, int sizeX, int sizeY)
{
	SpriteFile sprFile = Resource::LoadSprite(filename);
	this->posx = sprFile.x;
	this->posy = sprFile.y;
	this->w = sprFile.width;
	this->h = sprFile.height;
	this->image = sprFile.images;
	this->size = { sizeX,sizeY };
	this->isEmpty = this->image.empty();

	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;
}

noa::Sprite::Sprite() {
	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;
	this->isEmpty = this->image.empty();
}

noa::Sprite::Sprite(int w, int h, const Vector<int>& size, const std::vector<uint32_t>& image)
{
	this->w = w;
	this->h = h;
	this->image = image;
	this->size = size;
	this->isEmpty = this->image.empty();

	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;
}

noa::Sprite::~Sprite()
{

}

std::vector<uint32_t> noa::Sprite::GetImage()
{
	return this->image;
}

void noa::Sprite::UpdateImage(const SpriteFile& image)
{
	this->posx = image.x;
	this->posx = image.y;
	this->w = image.width;
	this->h = image.height;
	this->image = image.images;
	this->isEmpty = this->image.empty();

	this->sw = (w / 2) * 2;
	this->sh = (h / 2) * 2;
}

void noa::Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha) const
{
	//��spriteͼƬ��䵽������
	const int x1 = posX;
	const int y1 = posY;
	const int x2 = posX + size.x;
	const int y2 = posY + size.y;

	for (int x = min(x1, x2); x <= max(x1, x2); x++)
	{
		for (int y = min(y1, y2); y <= max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			const uint32_t color = GetColor(simple.x, simple.y);
			if (isRenderAlpha && (color == ERRORCOLOR || GetAValue(color) == 0))
			{
				continue;
			}
			renderer->DrawPixel(x, y, color);
			//renderer.DrawPixel(x, y, color);
		}
	}

}

void noa::Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const
{

	const int x1 = posX;
	const int y1 = posY;
	const int x2 = posX + size.x;
	const int y2 = posY + size.y;

	for (int x = min(x1, x2); x <= max(x1, x2); x++)
	{
		for (int y = min(y1, y2); y <= max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			uint32_t pixelColor = GetColor(simple.x, simple.y);

			if (isMirror)
			{
				//��ת
				pixelColor = GetColor(1 - simple.x, simple.y);
				//pixelColor = GetTransposeColor(simple.y, 1 - simple.x);

			}

			if (isRenderAlpha && GetAValue(pixelColor) == 0)
			{
				continue;
			}
			renderer->DrawPixel(x, y, pixelColor);
		}
	}

}

void noa::Sprite::DrawSprite(bool isRenderAlpha) const
{

	const int x1 = posx;
	const int y1 = posy;
	const int x2 = posx + size.x;
	const int y2 = posy + size.y;

	for (int x = min(x1, x2); x <= max(x1, x2); x++)
	{
		for (int y = min(y1, y2); y <= max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			const uint32_t color = GetColor(simple.x, simple.y);
			if (isRenderAlpha && GetAValue(color) == 0)
			{
				continue;
			}
			renderer->DrawPixel(x, y, color);
		}
	}

}

void noa::Sprite::DrawSprite(bool isRenderAlpha, bool isMirror) const
{

	const int x1 = posx;
	const int y1 = posy;
	const int x2 = posx + size.x;
	const int y2 = posy + size.y;

	for (int x = min(x1, x2); x <= max(x1, x2); x++)
	{
		for (int y = min(y1, y2); y <= max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			uint32_t pixelColor = GetColor(simple.x, simple.y);

			if (isMirror)
			{
				//��ת
				pixelColor = GetColor(1 - simple.x, simple.y);

			}

			if (isRenderAlpha && GetAValue(pixelColor) == 0)
			{
				continue;
			}
			renderer->DrawPixel(x, y, pixelColor);
		}
	}

}

void noa::Sprite::DrawSpriteFull()
{
	//ȫ������ͼƬ
	const float dx = 1.0f / Screen::width;
	const float dy = 1.0f / Screen::height;

	for (int x = 0; x < Screen::width; x++)
	{
		for (int y = 0; y < Screen::height; y++)
		{
			const uint32_t color = GetColor(x * dx, y * dy);
			renderer->DrawPixel(x, y, color);
		}
	}
}

void noa::Sprite::SetPixelColor(int x,int y,uint32_t color) 
{
	if (x<0||x>=w||y<0||y>=h)
	{
		return;
	}
	image[y * w + x] = color;
}

uint32_t noa::Sprite::GetPixelColor(const int x, const int y) const
{
	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return 0;
	}
	const int width = static_cast<int>(w);
	return image[y * width + x];
}

uint32_t noa::Sprite::GetColor(const float normalizedX, const float normalizedY) const
{
	if (isEmpty)
	{
		return ERRORCOLOR;
	}

	const int sx = static_cast<int>(std::abs(normalizedX * (w - 1))) % w;
	const int sy = static_cast<int>(std::abs(normalizedY * (h - 1))) % h;

	const int width = static_cast<int>(w);
	return image[sy * width + sx];

	/*if (isEmpty)
	{
		return ERRORCOLOR;
	}

	const float x = normalizedX * (w - 1);
	const float y = normalizedY * (h - 1);

	const int x0 = static_cast<int>(std::floor(x));
	const int y0 = static_cast<int>(std::floor(y));
	const int x1 = x0 + 1;
	const int y1 = y0 + 1;

	const float dx = x - x0;
	const float dy = y - y0;

	const Uint32 c00 = image[y0 * w + x0];
	const Uint32 c01 = image[y0 * w + x1];
	const Uint32 c10 = image[y1 * w + x0];
	const Uint32 c11 = image[y1 * w + x1];

	const Uint8 r = static_cast<Uint8>((1 - dx) * (1 - dy) * GetRValue(c00) + dx * (1 - dy) * GetRValue(c01) + (1 - dx) * dy * GetRValue(c10) + dx * dy * GetRValue(c11));
	const Uint8 g = static_cast<Uint8>((1 - dx) * (1 - dy) * GetGValue(c00) + dx * (1 - dy) * GetGValue(c01) + (1 - dx) * dy * GetGValue(c10) + dx * dy * GetGValue(c11));
	const Uint8 b = static_cast<Uint8>((1 - dx) * (1 - dy) * GetBValue(c00) + dx * (1 - dy) * GetBValue(c01) + (1 - dx) * dy * GetBValue(c10) + dx * dy * GetBValue(c11));
	const Uint8 a = static_cast<Uint8>((1 - dx) * (1 - dy) * GetAValue(c00) + dx * (1 - dy) * GetAValue(c01) + (1 - dx) * dy * GetAValue(c10) + dx * dy * GetAValue(c11));

	return RGBA(r, g, b, a);*/

}

uint32_t noa::Sprite::GetTransposeColor(const float normalizedX, const float normalizedY) const
{
	//��ͼƬ�л�ȡ����
	if (isEmpty)
	{
		return BLACK;
	}

	const int sx = static_cast<int>(std::abs((int)(normalizedX * (h - 1)))) % h;
	const int sy = static_cast<int>(std::abs((int)(normalizedY * (w - 1)))) % w;

	const int height = static_cast<int>(h);
	return image[sy * height + sx];
}

uint32_t noa::Sprite::GetTransposeColor(const Vector<float>& simple) const
{
	//��ͼƬ�л�ȡ����
	if (isEmpty)
	{
		return BLACK;
	}

	const int sx = static_cast<int>(std::abs(((int)(simple.x * (h - 1))))) % h;
	const int sy = static_cast<int>(std::abs(((int)(simple.y * (w - 1))))) % w;

	const int height = static_cast<int>(h);
	return image[sy * height + sx];
}

noa::SpriteGPU::SpriteGPU(Sprite* sprite)
{
	if (sprite == nullptr || renderer == nullptr)
	{
		return;
	}
	this->texture = renderer->CreateTexture(sprite->w, sprite->h, sprite->GetImage().data());
	this->size = sprite->size;
	texture->EnableAlpha();

}

noa::SpriteGPU::SpriteGPU(const SpriteFile& spriteFile, int sizeX, int sizeY) {
	this->texture = renderer->CreateTexture(
		spriteFile.width, spriteFile.height
		, (uint32_t*)spriteFile.images.data()
	);
	this->size.x = sizeX;
	this->size.y = sizeY;
	texture->EnableAlpha();
}

noa::SpriteGPU::~SpriteGPU()
{
	if (texture != nullptr)
	{
		delete texture;
	}
}

std::shared_ptr<noa::SpriteGPU> noa::SpriteGPU::Create(Sprite* sprite)
{
	return std::make_shared<SpriteGPU>(sprite);
}

std::shared_ptr<noa::SpriteGPU> noa::SpriteGPU::Create(const SpriteFile& spriteFile, int scaleX, int scaleY)
{
	return std::make_shared<SpriteGPU>(spriteFile,scaleX,scaleY);
}

void noa::SpriteGPU::SetLayer(InstanceLayer layer)
{
	this->layer = layer;
}

void noa::SpriteGPU::Update(Sprite* sprite)
{

	if (sprite != nullptr)
	{
		texture->UpdateTexture(sprite->GetImage().data(), sprite->w, sprite->h);
		this->size = sprite->size;
	}

}

void noa::SpriteGPU::DrawSprite(float x, float y, bool mirror, float eulerAngle)
{

	if (texture == nullptr)
	{
		return;
	}

	SpriteGPUInstance instance;
	instance.texture = texture;
	instance.position.x = static_cast<int>(x);
	instance.position.y = static_cast<int>(y);
	instance.size.x = this->size.x;
	instance.size.y = this->size.y;
	instance.tint = WHITE;
	instance.eulerAngle = eulerAngle;
	instance.flip = mirror;
	rendererInstanceLayer[static_cast<int>(layer)].push_back(instance);

}

void noa::SpriteGPU::DrawSprite(float x, float y, float w, float h, unsigned int tint, bool mirror, float eulerAngle)
{
	if (texture == nullptr)
	{
		return;
	}

	SpriteGPUInstance instance;
	instance.texture = texture;
	instance.position.x = static_cast<int>(x);
	instance.position.y = static_cast<int>(y);
	instance.size.x = static_cast<int>(w);
	instance.size.y = static_cast<int>(h);
	instance.eulerAngle = eulerAngle;
	instance.tint = tint;
	instance.flip = mirror;
	rendererInstanceLayer[static_cast<unsigned int>(layer)].push_back(instance);

}

