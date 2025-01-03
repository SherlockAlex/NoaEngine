#include "Sprite.h"
#include "Texture.h"
#include "Renderer.h"
#include "Graphic.h"
#include "Screen.h"

#include "Debug.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

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

}

noa::Sprite::Sprite() {
	this->isEmpty = this->image.empty();
}

noa::Sprite::Sprite(int w, int h, const Vector<int>& size, const std::vector<uint32_t>& image)
{
	this->w = w;
	this->h = h;
	this->image = image;
	this->size = size;
	this->isEmpty = this->image.empty();

}

noa::Sprite::~Sprite()
{

}

std::vector<uint32_t> noa::Sprite::GetImage() const
{
	return this->image;
}

void noa::Sprite::ResizeAndFull(int w,int h,uint32_t color) 
{
	this->w = w;
	this->h = h;
	this->image.resize(w*h,color);
	this->isEmpty = image.empty();
}

void noa::Sprite::Full(uint32_t color) 
{
	for (int i = 0;i<w*h;i++) 
	{
		image[i] = color;
	}
	this->isEmpty = image.empty();
}

void noa::Sprite::UpdateImage(const SpriteFile& image)
{
	this->posx = image.x;
	this->posx = image.y;
	this->w = image.width;
	this->h = image.height;
	this->image = image.images;
	this->isEmpty = this->image.empty();
}

void noa::Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha) const
{
	//将sprite图片填充到矩形上
	const int x1 = posX;
	const int y1 = posY;
	const int x2 = posX + size.x;
	const int y2 = posY + size.y;

	for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
	{
		for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++)
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

	for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
	{
		for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			uint32_t pixelColor = GetColor(simple.x, simple.y);

			if (isMirror)
			{
				//翻转
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

	for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
	{
		for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++)
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

	for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
	{
		for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			uint32_t pixelColor = GetColor(simple.x, simple.y);

			if (isMirror)
			{
				//翻转
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
	//全屏绘制图片
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

void noa::Sprite::MapFromSprite(const noa::Sprite& sprite)
{
	
	for (int x = 0; x < w;x++) {
		for (int y = 0; y < h;y++)
		{
			const uint32_t ownColor = GetPixelColor(x,y);
			const uint8_t alpha = noa::GetAValue(ownColor);
			const float simpleX = static_cast<float>(x) / w;
			const float simpleY = static_cast<float>(y) / h;

			const float alphaValue = static_cast<float>(alpha) / 255.0f;

			const uint32_t simpleColor = 
				sprite.GetColor(simpleX,simpleY);
			const uint32_t color = noa::RGBA(
				noa::GetRValue(simpleColor)
				, noa::GetGValue(simpleColor)
				, noa::GetBValue(simpleColor)
				, static_cast<uint8_t>(noa::GetAValue(simpleColor) * alphaValue)
			);

			SetPixelColor(x,y,color);

		}
	}
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

}

uint32_t noa::Sprite::GetTransposeColor(const float normalizedX, const float normalizedY) const
{
	//从图片中获取像素
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
	//从图片中获取像素
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
		, (void*)spriteFile.images.data()
	);
	this->size.x = sizeX;
	this->size.y = sizeY;
	texture->EnableAlpha();
}

noa::SpriteGPU::SpriteGPU(void* pixels,int width,int height) 
{
	this->texture = renderer->CreateTexture(width,height,pixels);
	this->size.x = width;
	this->size.y = height;
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

std::shared_ptr<noa::SpriteGPU> noa::SpriteGPU::Create(void* pixels,int width,int height) 
{
	return std::make_shared<SpriteGPU>(pixels,width,height);
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

void noa::SpriteGPU::UpdateImage(const noa::SpriteFile& image)
{
	texture->UpdateTexture(image.images.data(), image.width, image.height);
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
	rendererInstanceLayer[static_cast<uint32_t>(layer)].push_back(instance);

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
	rendererInstanceLayer[static_cast<uint32_t>(layer)].push_back(instance);

}

