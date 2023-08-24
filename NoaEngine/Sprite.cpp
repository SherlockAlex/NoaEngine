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

	Sprite::Sprite(int w, int h, int size, vector<Uint32> image)
	{
		this->w = w;
		this->h = h;
		this->image = image;
		this->sizeForSurface = size;
	}

	Sprite::~Sprite()
	{

	}

	/*void Sprite::UpdateImage(Uint32* image)
	{
		this->image = image;
	}*/

	void Sprite::UpdateImage(vector<Uint32> image)
	{
		this->image = image;
	}

	void Sprite::UpdateImage(const SpriteFile & image)
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
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NoaAbs<int>(normalizedX * (float)w) % w;
		const int sy = NoaAbs<int>(normalizedY * (float)h - 1) % h;

		return image[sy * w + sx];
	}

	Uint32 Sprite::GetTransposeColor(float normalizedX, float normalizedY) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NoaAbs<int>(normalizedX * (float)h)%h;
		const int sy = NoaAbs<int>(normalizedY * (float)w)%w;
		//const int sy = NoaAbs<int>(normalizedY * (float)w - 1)%w;


		return image[sy * h + sx];
	}

	Uint32 Sprite::GetTransposeColor(const Vector<float> & simple) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NoaAbs<int>(simple.x * (float)h) % h;
		const int sy = NoaAbs<int>(simple.y * (float)w - 1) % w;

		return image[sy * h + sx];
	}
}

