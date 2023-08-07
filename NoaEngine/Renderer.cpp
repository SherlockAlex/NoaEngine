#include "Renderer.h"
//#include "Sprite.h"

Renderer::Renderer()
{
}

Renderer::Renderer(int pixelWidth, int pixelHeight, void* pixelBuffer)
{
	this->pixelWidth = pixelWidth;
	this->pixelHeight = pixelHeight;
	this->pixelBuffer = (Uint32*)pixelBuffer;
}

void Renderer::DrawPixel(int x, int y, Uint32 color)
{
	if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
	{
		return;
	}
	pixelBuffer[y * pixelWidth + x] = color;
}

//void Renderer::DrawImage(int posX, int posY, int imageW, int imageH, int scaleForSurface, bool isDrawAlpha, Uint32* imageRGB)
//{
//	//º∆À„∑≈¥Û
//	int wannaW = pixelWidth / scaleForSurface;
//	int wannaH = (int)(((float)imageH / (float)imageW) * wannaW);
//
//	Sprite sprite = Sprite(imageW, imageH, 1, imageRGB);
//
//	for (int width = 0; width < wannaW; width++)
//	{
//		for (int height = 0; height < wannaH; height++)
//		{
//			int x = posX - ((float)wannaW * 0.5f) + width;
//			int y = posY - ((float)wannaH * 0.5f) + height;
//
//			float fSimpleX = (float)(width) / (float)wannaW;
//			float fSimpleY = (float)(height) / (float)wannaH;
//
//			//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, imageH, inmageW, imageRGB);
//			Uint32 pixelColor = sprite.GetColor(fSimpleY, fSimpleX);
//			if (isDrawAlpha)
//			{
//				if (pixelColor == BLACK)
//				{
//					continue;
//				}
//			}
//
//			if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
//			{
//				continue;
//			}
//			DrawPixel(x, y, pixelColor);
//		}
//	}
//}
