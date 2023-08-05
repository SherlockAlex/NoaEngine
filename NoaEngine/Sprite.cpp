#include "Sprite.h"
#include "NoaEngine.h"

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

/// <summary>
/// 绘制Sprite到屏幕相应位置
/// </summary>
/// <param name="posX">屏幕坐标x</param>
/// <param name="posY">屏幕坐标y</param>
/// <param name="isRenderAlpha">是否采用alpha通道</param>
void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha)
{
	//计算放大
	int wannaW = game.PixelWidth() / sizeForSurface;
	int wannaH = (int)(((float)h / (float)w) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
			Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

			if (isRenderAlpha)
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}

			if (x < 0 || x >= game.PixelWidth() || y < 0 || y >= game.PixelHeight())
			{
				continue;
			}
			game.renderer.DrawPixel(x, y, pixelColor);
		}
	}
}

void Sprite::DrawSprite(bool isRenderAlpha)
{
	//计算放大
	int wannaW = game.PixelWidth() / sizeForSurface;
	int wannaH = (int)(((float)h / (float)w) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posx - ((float)wannaW * 0.5f) + width;
			int y = posy - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
			Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

			if (isRenderAlpha)
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}

			if (x < 0 || x >= game.PixelWidth() || y < 0 || y >= game.PixelHeight())
			{
				continue;
			}
			game.renderer.DrawPixel(x, y, pixelColor);
			//DrawPixel(x, y, pixelColor);
		}
	}
}

Uint32 Sprite::GetColor(float normalizedX, float normalizedY)
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

Uint32 Sprite::GetTransposeColor(float normalizedX, float normalizedY) {
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