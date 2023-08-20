#include "Sprite.h"
#include "NoaEngine.h"

namespace noa {
	extern Renderer renderer;

	///Sprite类的实现
	Sprite::Sprite(SpriteFile sprFile, int sizeForSurface)
	{
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->sizeForSurface = sizeForSurface;
	}

	Sprite::Sprite(const char* file, int sizeForSurface) {
		this->sizeForSurface = sizeForSurface;
		SpriteFile sprFile = LoadSprFile(file);
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
	}

	Sprite::Sprite() {

	}

	Sprite::Sprite(int w, int h, int size, Uint32* image)
	{
		this->w = w;
		this->h = h;
		this->image = image;
		this->sizeForSurface = size;
	}

	Sprite::~Sprite()
	{

	}

	void Sprite::UpdateImage(Uint32* image)
	{
		this->image = image;
	}

	void Sprite::UpdateImage(SpriteFile image)
	{
		this->posx = image.x;
		this->posx = image.y;
		this->w = image.width;
		this->h = image.height;
		this->image = image.images;
		//this->sizeForSurface = sizeForSurface;
	}

	/// <summary>
	/// 绘制Sprite到屏幕相应位置
	/// </summary>
	/// <param name="posX">屏幕坐标x</param>
	/// <param name="posY">屏幕坐标y</param>
	/// <param name="isRenderAlpha">是否采用alpha通道</param>
	void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha) const
	{
		//计算放大
		const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);

		for (int width = 0; width < wannaW; width++)
		{
			for (int height = 0; height < wannaH; height++)
			{
				const int x = posX - ((float)wannaW * 0.5f) + width;
				const int y = posY - ((float)wannaH * 0.5f) + height;

				const float fSimpleX = (float)(width) / (float)wannaW;
				const float fSimpleY = (float)(height) / (float)wannaH;

				//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
				const Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}

				if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
				{
					continue;
				}
				renderer.DrawPixel(x, y, pixelColor);
			}
		}
	}

	void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const
	{
		//计算放大
		const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);

		for (int width = 0; width < wannaW; width++)
		{
			for (int height = 0; height < wannaH; height++)
			{
				const int x = posX - ((float)wannaW * 0.5f) + width;
				const int y = posY - ((float)wannaH * 0.5f) + height;

				const float fSimpleX = (float)(width) / (float)wannaW;
				const float fSimpleY = (float)(height) / (float)wannaH;

				//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
				Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

				if (isMirror)
				{
					//翻转
					pixelColor = GetTransposeColor(fSimpleY, 1 - fSimpleX);
				}

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}

				if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
				{
					continue;
				}
				renderer.DrawPixel(x, y, pixelColor);
			}
		}
	}

	void Sprite::DrawSprite(bool isRenderAlpha) const
	{
		//计算放大
		const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);

		for (int width = 0; width < wannaW; width++)
		{
			for (int height = 0; height < wannaH; height++)
			{
				const int x = posx - ((float)wannaW * 0.5f) + width;
				const int y = posy - ((float)wannaH * 0.5f) + height;

				const float fSimpleX = (float)(width) / (float)wannaW;
				const float fSimpleY = (float)(height) / (float)wannaH;

				const Uint32 pixelColor = GetTransposeColor(fSimpleY, 1 - fSimpleX);

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}

				if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
				{
					continue;
				}
				renderer.DrawPixel(x, y, pixelColor);
			}
		}
	}

	void Sprite::DrawSprite(bool isRenderAlpha, bool isMirror) const
	{
		//计算放大
		const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);

		for (int width = 0; width < wannaW; width++)
		{
			for (int height = 0; height < wannaH; height++)
			{
				const int x = posx - ((float)wannaW * 0.5f) + width;
				const int y = posy - ((float)wannaH * 0.5f) + height;

				const float fSimpleX = (float)(width) / (float)wannaW;
				const float fSimpleY = (float)(height) / (float)wannaH;


				Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

				if (isMirror)
				{
					//翻转
					pixelColor = GetTransposeColor(1 - fSimpleY, fSimpleX);
				}

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}

				if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
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
		const float dx = 1.0 / (float)pixelWidth;
		const float dy = 1.0 / (float)pixelHeight;
		
		for (int x = 0;x < pixelWidth;x++) 
		{
			for (int y = 0;y < pixelHeight;y++) 
			{
				const Uint32 color = GetTransposeColor(y*dy, x*dx);
				renderer.DrawPixel(x,y,color);
			}
		}
	}

	Uint32 Sprite::GetColor(float normalizedX, float normalizedY) const
	{
		//从图片中获取像素
		int sx = normalizedX * (float)w;
		int sy = normalizedY * (float)h - 1;
		//printf("%d,%d\n", sx, sy);
		if (sx<0 || sx>w || sy<0 || sy>h)
		{
			sx = sx - (int)sx;
			sy = sy - (int)sy;
			return BLACK;
		}

		sx = sx & (w - 1);
		sy = sy & (h - 1);

		return image[sy * w + sx];
	}

	Uint32 Sprite::GetTransposeColor(float normalizedX, float normalizedY) const
	{
		//从图片中获取像素
		int sx = normalizedX * (float)h;
		int sy = normalizedY * (float)w - 1;
		//printf("%d,%d\n", sx, sy);
		if (sx<0 || sx>h || sy<0 || sy>w)
		{
			sx = sx - (int)sx;
			sy = sy - (int)sy;
			return BLACK;
		}

		/*sx = sx & (h - 1);
		sy = sy & (w - 1);*/

		return image[sy * h + sx];
	}
}

