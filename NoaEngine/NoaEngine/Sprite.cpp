#include "Sprite.h"
#include "NoaEngine.h"


using namespace std;

namespace noa {

	extern shared_ptr<Renderer> renderer;

	Sprite::Sprite(const SpriteFile& sprFile, const Vector<int>& scale)
	{
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->scale = scale;

		this->isEmpty = this->image.empty();

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
	}

	Sprite::Sprite(const Vector<int>& scale)
	{
		this->scale = scale;
		this->isEmpty = this->image.empty();
	}

	Sprite::Sprite(const char* file, const Vector<int>& scale) {
		SpriteFile sprFile = resource.LoadSprFile(file);
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->scale = scale;
		this->isEmpty = this->image.empty();

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;

	}

	Sprite::Sprite() {
		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
		this->isEmpty = this->image.empty();
	}

	Sprite::Sprite(int w, int h,const Vector<int> & scale,const std::vector<Uint32> & image)
	{
		this->w = w;
		this->h = h;
		this->image = image;
		this->scale = scale;
		this->isEmpty = this->image.empty();

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
	}

	Sprite::~Sprite()
	{
		
	}

	std::vector<uint32_t> Sprite::GetImage()
	{
		return this->image;
	}

	void Sprite::UpdateImage(const SpriteFile & image)
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

	void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha) const
	{
		//将sprite图片填充到矩形上
		const int x1 = posX;
		const int y1 = posY;
		const int x2 = posX + scale.x;
		const int y2 = posY + scale.y;

		for (int x = min(x1, x2); x <= max(x1, x2); x++)
		{
			for (int y = min(y1, y2); y <= max(y1, y2); y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = GetColor(simple.x, simple.y);
				if (isRenderAlpha&& (color == ERRORCOLOR||GetAValue(color) == 0))
				{
					continue;
				}
				DRAWPIXEL(x,y,color);
				//renderer.DrawPixel(x, y, color);
			}
		}

	}

	void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const
	{

		const int x1 = posX;
		const int y1 = posY;
		const int x2 = posX + scale.x;
		const int y2 = posY + scale.y;

		for (int x = min(x1, x2); x <= max(x1, x2); x++)
		{
			for (int y = min(y1, y2); y <= max(y1, y2); y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				Uint32 pixelColor = GetColor(simple.x, simple.y);

				if (isMirror)
				{
					//翻转
					pixelColor = GetColor(1-simple.x,simple.y);
					//pixelColor = GetTransposeColor(simple.y, 1 - simple.x);

				}

				if (isRenderAlpha&& GetAValue(pixelColor) == 0)
				{
					continue;
				}
				renderer->DrawPixel(x, y, pixelColor);
			}
		}

	}

	void Sprite::DrawSprite(bool isRenderAlpha) const
	{

		const int x1 = posx;
		const int y1 = posy;
		const int x2 = posx + scale.x;
		const int y2 = posy + scale.y;

		for (int x = min(x1, x2); x <= max(x1, x2); x++)
		{
			for (int y = min(y1, y2); y <= max(y1, y2); y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = GetColor(simple.x,simple.y);
				if (isRenderAlpha && GetAValue(color) == 0)
				{
					continue;
				}
				renderer->DrawPixel(x, y, color);
			}
		}

	}

	void Sprite::DrawSprite(bool isRenderAlpha, bool isMirror) const
	{

		const int x1 = posx;
		const int y1 = posy;
		const int x2 = posx + scale.x;
		const int y2 = posy + scale.y;

		for (int x = min(x1, x2); x <= max(x1, x2); x++)
		{
			for (int y = min(y1, y2); y <= max(y1, y2); y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				Uint32 pixelColor = GetColor(simple.x, simple.y);

				if (isMirror)
				{
					//翻转
					pixelColor = GetColor(1-simple.x, simple.y);

				}

				if (isRenderAlpha && GetAValue(pixelColor) == 0)
				{
					continue;
				}
				renderer->DrawPixel(x, y, pixelColor);
			}
		}

	}

	void Sprite::DrawSpriteFull()
	{
		//全屏绘制图片
		const float dx = 1.0f / pixelWidth;
		const float dy = 1.0f / pixelHeight;
		
		for (int x = 0;x < pixelWidth;x++) 
		{
			for (int y = 0;y < pixelHeight;y++) 
			{
				const Uint32 color = GetColor(x*dx, y*dy);
				renderer->DrawPixel(x,y,color);
			}
		}
	}

	Uint32 Sprite::GetPixelColor(const int x,const int y) const
	{
		if (x<0||x>=w||y<0||y>=h)
		{
			return 0;
		}
		return image[y * w + x];
	}

	Uint32 Sprite::GetColor(const float normalizedX,const float normalizedY) const
	{
		if (isEmpty)
		{
			return ERRORCOLOR;
		}

		

		const int sx = static_cast<int>(std::abs(normalizedX * (w - 1))) % w;
		const int sy = static_cast<int>(std::abs(normalizedY * (h - 1))) % h;

		return image[sy * w + sx];
	}

	Uint32 Sprite::GetTransposeColor(const float normalizedX,const float normalizedY) const
	{
		//从图片中获取像素
		if (isEmpty)
		{
			return BLACK;
		}

		const int sx = NOAABS((int)(normalizedX * (h-1))) % h;
		const int sy = NOAABS((int)(normalizedY * (w-1))) % w;

		return image[sy * h + sx];
	}

	Uint32 Sprite::GetTransposeColor(const Vector<float> & simple) const
	{
		//从图片中获取像素
		if (isEmpty)
		{
			return BLACK;
		}

		const int sx = NOAABS((int)(simple.x * (h-1))) % h;
		const int sy = NOAABS((int)(simple.y * (w-1))) % w;

		return image[sy * h + sx];
	}

	SpriteGPU::SpriteGPU(Sprite* sprite)
	{
		if (sprite == nullptr||renderer == nullptr) 
		{
			return;
		}
		this->sprite = sprite;
		this->texture = renderer->CreateTexture(sprite->w, sprite->h, sprite->GetImage().data());
		texture->EnableAlpha();

	}

	SpriteGPU::~SpriteGPU()
	{
		if (texture!=nullptr) 
		{
			delete texture;
		}
	}

	void SpriteGPU::SetLayer(InstanceLayer layer)
	{
		this->layer = layer;
	}

	void SpriteGPU::Update(Sprite* sprite)
	{

		if (sprite != nullptr)
		{
			texture->UpdateTexture(sprite->GetImage().data(), sprite->w, sprite->h);
		}

	}

	void SpriteGPU::DrawSprite(float x, float y, bool mirror, float eulerAngle)
	{

		if (texture == nullptr || sprite == nullptr)
		{
			return;
		}

		texture->UpdateTexture(sprite->GetImage().data(), sprite->w, sprite->h);
		SpriteGPUInstance instance;
		instance.texture = texture;
		instance.position.x = static_cast<int>(x);
		instance.position.y = static_cast<int>(y);
		instance.scale.x = sprite->scale.x;
		instance.scale.y = sprite->scale.y;
		instance.tint = WHITE;
		instance.eulerAngle = eulerAngle;
		instance.flip = mirror;
		rendererInstanceLayer[static_cast<int>(layer)].push_back(instance);

	}

	void SpriteGPU::DrawSprite(float x, float y, float w, float h, unsigned int tint, bool mirror, float eulerAngle)
	{
		if (texture == nullptr || sprite == nullptr)
		{
			return;
		}

		texture->UpdateTexture(sprite->GetImage().data(), sprite->w, sprite->h);
		SpriteGPUInstance instance;
		instance.texture = texture;
		instance.position.x = static_cast<int>(x);
		instance.position.y = static_cast<int>(y);
		instance.scale.x = static_cast<int>(w);
		instance.scale.y = static_cast<int>(h);
		instance.eulerAngle = eulerAngle;
		instance.tint = tint;
		instance.flip = mirror;
		rendererInstanceLayer[static_cast<unsigned int>(layer)].push_back(instance);
	}

}

