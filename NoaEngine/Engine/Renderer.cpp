#include "Graphic.h"
#include "Renderer.h"
#include "Sprite.h"
#include "FontAsset.h"
#include "Font.h"
#include "NoaMath.h"
#include "Debug.h"
#include "Screen.h"

#include <algorithm>

std::shared_ptr<noa::FontAsset> fontAsset;

noa::Renderer::Renderer()
{

}

void noa::Renderer::InitFontAsset() {
	// 初始化字体
	fontAsset = std::make_shared<FontAsset>("./Data/Resource/font/font.ttf", 48);
}

void noa::Renderer::SetRenderer() {
	this->pixelWidth = noa::Screen::width;
	this->pixelHeight = noa::Screen::height;
	this->pixelBuffer = noa::Screen::pixelBuffer;

	this->invPixelWidth = 1.0f / noa::Screen::width;
	this->invPixelHeight = 1.0f / noa::Screen::height;
}

void noa::Renderer::DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color) const
{
	if (x >= static_cast<uint32_t>(pixelWidth) || y >= static_cast<uint32_t>(pixelHeight))
	{
		return;
	}

	((uint32_t*)pixelBuffer)[y * pixelWidth + x] = color;

}

void noa::Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Uint32 color) const
{
	const int x1 = point1.x;
	const int y1 = point1.y;
	const int x2 = point2.x;
	const int y2 = point2.y;


	const int minX = Min(x1, x2);
	const int maxX = Max(x1, x2);
	const int minY = Min(y1, y2);
	const int maxY = Max(y1, y2);

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			DrawPixel(x, y, color);
		}
	}
}

void noa::Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite) const
{
	//将sprite图片填充到矩形上
	const int x1 = point1.x;
	const int y1 = point1.y;
	const int x2 = point2.x;
	const int y2 = point2.y;

	const int minX = Min(x1, x2);
	const int maxX = Max(x1, x2);
	const int minY = Min(y1, y2);
	const int maxY = Max(y1, y2);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			const uint32_t color = sprite.GetColor(simple.x, simple.y);
			DrawPixel(x, y, color);
		}
	}
}

void noa::Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite, Uint32 mutiColor, bool isAlpha) const
{
	//将sprite图片填充到矩形上
	const int x1 = point1.x;
	const int y1 = point1.y;
	const int x2 = point2.x;
	const int y2 = point2.y;

	const int minX = Min(x1, x2);
	const int maxX = Max(x1, x2);
	const int minY = Min(y1, y2);
	const int maxY = Max(y1, y2);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			const uint32_t color = sprite.GetColor(simple.x, simple.y);
			if (isAlpha && GetAValue(color) == 0)
			{
				continue;
			}

			DrawPixel(x, y, mutiColor);
		}
	}
}

void noa::Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite* sprite, Uint32 mutiColor, bool isAlpha) const
{
	//将sprite图片填充到矩形上
	const int x1 = point1.x;
	const int y1 = point1.y;
	const int x2 = point2.x;
	const int y2 = point2.y;

	const int minX = Min(x1, x2);
	const int maxX = Max(x1, x2);
	const int minY = Min(y1, y2);
	const int maxY = Max(y1, y2);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			const Vector<float> simple(
				(float)(x - x1) / (x2 - x1),
				(float)(y - y1) / (y2 - y1)
			);
			uint32_t color = sprite->GetColor(simple.x, simple.y);

			if (isAlpha && GetAValue(color) == 0)
			{
				continue;
			}

			color = RGBA(
				static_cast<unsigned char>(GetRValue(color) * (GetRValue(mutiColor) / 255.0))
				, static_cast<unsigned char>(GetGValue(color) * (GetGValue(mutiColor) / 255.0))
				, static_cast<unsigned char>(GetBValue(color) * (GetBValue(mutiColor) / 255.0))
				, static_cast<unsigned char>(GetAValue(color))
			);

			DrawPixel(x, y, color);
		}
	}
}

void noa::Renderer::DrawString(
	const std::wstring& str
	, int x
	, int y
	, Uint32 color
	, int size
)
{
	int row = 0;

	const int length = static_cast<int>(str.length());
	//const char* c_str = str.c_str();

	const float scale = static_cast<float>(size) / fontAsset->size;

	float tempX = static_cast<float>(x);
	float tempY = static_cast<float>(y);

	for (int i = 0; i < length; i++)
	{

		const wchar_t c = str[i];
		if (c == '\n')
		{
			tempX = static_cast<float>(x);
			row++;
			continue;
		}
		const Font* font = fontAsset->GetFont(c);
		if (font == nullptr)
		{
			continue;
		}

		float w = font->w * scale;
		float h = font->h * scale;

		float posX = tempX + font->bearing.x * scale;
		float posY = tempY - font->bearing.y * scale + (row+1)*size;

		font->spriteGPU->DrawSprite(
			posX
			, posY
			, w
			, h
			, color
			, false
			, 0.0f);

		tempX = tempX + static_cast<float>(font->advance >> 6) * scale;
	}

}

noa::Vector<int> noa::Renderer::GetLabelScale(const std::wstring& str, int size)
{
	int row = 0;

	const int length = static_cast<int>(str.length());

	const float scale = static_cast<float>(size) / fontAsset->size;

	float tempX = 0;
	float tempY = 0;

	Vector<int> result;

	for (int i = 0; i < length; i++)
	{

		const wchar_t c = str[i];
		if (c == '\n')
		{
			tempX = 0;
			row++;
			continue;
		}
		const Font* font = fontAsset->GetFont(c);
		if (font == nullptr)
		{
			continue;
		}

		float w = font->w * scale;
		float h = font->h * scale;

		float posX = tempX + font->bearing.x * scale;
		float posY = tempY - font->bearing.y * scale + (row + 1) * size;

		tempX = tempX + (font->advance >> 6) * scale;
		int textScaleX = static_cast<int>(tempX);
		int textScaleY = static_cast<int>(row + 1) * size;
		if (result.x < textScaleX)
		{
			result.x = textScaleX;
		}
		result.y = textScaleY;
	}

	return result;
}

void noa::Renderer::FullScreen(Uint32 color) const
{
	for (int i = 0; i < pixelHeight * pixelWidth; i++)
	{
		((uint32_t*)pixelBuffer)[i] = color;
	}
}


