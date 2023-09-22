#include "Sprite.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {
	extern Renderer renderer;

	///Sprite类的实现
	Sprite::Sprite(SpriteFile sprFile, Vector<int> scale)
	{
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->scale = scale;

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
	}

	Sprite::Sprite(Vector<int> scale)
	{
		this->scale = scale;
	}

	Sprite::Sprite(const char* file, Vector<int> scale) {
		SpriteFile sprFile = resource.LoadSprFile(file);
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->scale = scale;

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;

	}

	Sprite::Sprite() {
		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
	}

	Sprite::Sprite(int w, int h, Vector<int> scale, vector<Uint32> image)
	{
		this->w = w;
		this->h = h;
		this->image = image;
		this->scale = scale;

		this->sw = (w / 2) * 2;
		this->sh = (h / 2) * 2;
	}

	Sprite::~Sprite()
	{
		Debug("Remove sprite successfully");
	}

	void Sprite::UpdateImage(const SpriteFile & image)
	{
		this->posx = image.x;
		this->posx = image.y;
		this->w = image.width;
		this->h = image.height;
		this->image = image.images;

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
				const uint32_t color = GetTransposeColor(simple.y, simple.x);
				if (isRenderAlpha&& color == ERRORCOLOR)
				{
					continue;
				}
				renderer.DrawPixel(x, y, color);
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
				Uint32 pixelColor = GetTransposeColor(simple.y, simple.x);

				if (isMirror)
				{
					//翻转
					pixelColor = GetTransposeColor(simple.y, 1 - simple.x);

				}

				if (isRenderAlpha&& pixelColor == ERRORCOLOR)
				{
					continue;
				}
				renderer.DrawPixel(x, y, pixelColor);
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
				const uint32_t color = GetTransposeColor(simple.y, simple.x);
				if (isRenderAlpha&& color == ERRORCOLOR)
				{
					continue;
				}
				renderer.DrawPixel(x, y, color);
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
				Uint32 pixelColor = GetTransposeColor(simple.y, simple.x);

				if (isMirror)
				{
					//翻转
					pixelColor = GetTransposeColor(simple.y, 1 - simple.x);

				}

				if (isRenderAlpha&& pixelColor == ERRORCOLOR)
				{
					continue;
				}
				renderer.DrawPixel(x, y, pixelColor);
			}
		}

	}

	void Sprite::DrawSpriteFull()
	{
		//全屏绘制图片
		const float dx = 1.0 / pixelWidth;
		const float dy = 1.0 / pixelHeight;
		
		for (int x = 0;x < pixelWidth;x++) 
		{
			for (int y = 0;y < pixelHeight;y++) 
			{
				const Uint32 color = GetTransposeColor(y*dy, x*dx);
				renderer.DrawPixel(x,y,color);
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
		if (image.empty())
		{
			return BLACK;
		}

		

		const int sx = NOAABS((int)(normalizedX * w)) % w;
		const int sy = NOAABS((int)(normalizedY * h)) % h;

		return image[sy * w + sx];
	}

	Uint32 Sprite::GetTransposeColor(const float normalizedX,const float normalizedY) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NOAABS((int)(normalizedX * h))%h;
		const int sy = NOAABS((int)(normalizedY * w))%w;

		return image[sy * h + sx];
	}

	Uint32 Sprite::GetTransposeColor(const Vector<float> & simple) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NOAABS((int)(simple.x * h)) % h;
		const int sy = NOAABS((int)(simple.y * w)) % w;

		return image[sy * h + sx];
	}

}

