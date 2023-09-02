#include "Sprite.h"
#include "NoaEngine.h"

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
		//this->sizeForSurface = sizeForSurface;
		this->scale = scale;
	}

	Sprite::Sprite(const char* file, Vector<int> scale) {
		//this->sizeForSurface = sizeForSurface;
		SpriteFile sprFile = LoadSprFile(file);
		this->posx = sprFile.x;
		this->posx = sprFile.y;
		this->w = sprFile.width;
		this->h = sprFile.height;
		this->image = sprFile.images;
		this->scale = scale;
	}

	Sprite::Sprite() {

	}

	Sprite::Sprite(int w, int h, Vector<int> scale, vector<Uint32> image)
	{
		this->w = w;
		this->h = h;
		this->image = image;
		this->scale = scale;
		//this->sizeForSurface = size;
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
		this->image = move(image);
	}

	void Sprite::UpdateImage(const SpriteFile & image)
	{
		this->posx = image.x;
		this->posx = image.y;
		this->w = image.width;
		this->h = image.height;
		this->image = move(image.images);
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
		//const int wannaW = pixelWidth * sizeForSurface;
		//const int wannaH = (int)(((float)h / (float)w) * wannaW);

		//const int wannaW = w * scale.x;
		//const int wannaH = h * scale.y;

		//for (int width = 0; width < wannaW; width++)
		//{
		//	for (int height = 0; height < wannaH; height++)
		//	{
		//		const int x = posX - ((float)wannaW * 0.5f) + width;
		//		const int y = posY - ((float)wannaH * 0.5f) + height;

		//		const float fSimpleX = (float)(width) / (float)wannaW;
		//		const float fSimpleY = (float)(height) / (float)wannaH;

		//		//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
		//		const Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

		//		if (isRenderAlpha)
		//		{
		//			if (pixelColor == BLACK)
		//			{
		//				continue;
		//			}
		//		}

		//		if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
		//		{
		//			continue;
		//		}
		//		renderer.DrawPixel(x, y, pixelColor);
		//	}
		//}

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
				if (isRenderAlpha)
				{
					if (color == BLACK)
					{
						continue;
					}
				}
				renderer.DrawPixel(x, y, color);
			}
		}

	}

	void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const
	{
		//计算放大

		/*const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);*/
		//const int wannaW = w * scale.x;
		//const int wannaH = h * scale.y;

		//for (int width = 0; width < wannaW; width++)
		//{
		//	for (int height = 0; height < wannaH; height++)
		//	{
		//		const int x = posX - ((float)wannaW * 0.5f) + width;
		//		const int y = posY - ((float)wannaH * 0.5f) + height;

		//		const float fSimpleX = (float)(width) / (float)wannaW;
		//		const float fSimpleY = (float)(height) / (float)wannaH;

		//		//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);
		//		Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

		//		if (isMirror)
		//		{
		//			//翻转
		//			pixelColor = GetTransposeColor(fSimpleY, 1 - fSimpleX);
		//		}

		//		if (isRenderAlpha)
		//		{
		//			if (pixelColor == BLACK)
		//			{
		//				continue;
		//			}
		//		}

		//		if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
		//		{
		//			continue;
		//		}
		//		renderer.DrawPixel(x, y, pixelColor);
		//	}
		//}

		/*const int x1 = posX - scale.x*0.5;
		const int y1 = posY - scale.y*0.5;
		const int x2 = posX + scale.x*0.5;
		const int y2 = posY + scale.y*0.5;*/

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

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}
				renderer.DrawPixel(x, y, pixelColor);
			}
		}

	}

	void Sprite::DrawSprite(bool isRenderAlpha) const
	{
		//计算放大
		/*const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);*/
		/*const int wannaW = w * scale.x;
		const int wannaH = h * scale.y;

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
		}*/

		/*const int x1 = posx - scale.x * 0.5;
		const int y1 = posy - scale.y * 0.5;
		const int x2 = posx + scale.x * 0.5;
		const int y2 = posy + scale.y * 0.5;*/

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
				if (isRenderAlpha)
				{
					if (color == BLACK)
					{
						continue;
					}
				}
				renderer.DrawPixel(x, y, color);
			}
		}

	}

	void Sprite::DrawSprite(bool isRenderAlpha, bool isMirror) const
	{
		//计算放大
		/*const int wannaW = pixelWidth / sizeForSurface;
		const int wannaH = (int)(((float)h / (float)w) * wannaW);*/
		//const int wannaW = w * scale.x;
		//const int wannaH = h * scale.y;

		//for (int width = 0; width < wannaW; width++)
		//{
		//	for (int height = 0; height < wannaH; height++)
		//	{
		//		const int x = posx - ((float)wannaW * 0.5f) + width;
		//		const int y = posy - ((float)wannaH * 0.5f) + height;

		//		const float fSimpleX = (float)(width) / (float)wannaW;
		//		const float fSimpleY = (float)(height) / (float)wannaH;


		//		Uint32 pixelColor = GetTransposeColor(fSimpleY, fSimpleX);

		//		if (isMirror)
		//		{
		//			//翻转
		//			pixelColor = GetTransposeColor(1 - fSimpleY, fSimpleX);
		//		}

		//		if (isRenderAlpha)
		//		{
		//			if (pixelColor == BLACK)
		//			{
		//				continue;
		//			}
		//		}

		//		if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
		//		{
		//			continue;
		//		}
		//		renderer.DrawPixel(x, y, pixelColor);
		//	}
		//}

		/*const int x1 = posx - scale.x * 0.5;
		const int y1 = posy - scale.y * 0.5;
		const int x2 = posx + scale.x * 0.5;
		const int y2 = posy + scale.y * 0.5;*/

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

				if (isRenderAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
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

		const int sx = NoaFastAbs(normalizedX * w) % w;
		const int sy = NoaFastAbs(normalizedY * h) % h;

		return image[sy * w + sx];
	}

	Uint32 Sprite::GetTransposeColor(const float normalizedX,const float normalizedY) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = (NoaFastAbs(normalizedX * h))%h;
		const int sy = (NoaFastAbs(normalizedY * w))%w;

		return image[sy * h + sx];
	}

	Uint32 Sprite::GetTransposeColor(const Vector<float> & simple) const
	{
		//从图片中获取像素
		if (image.empty())
		{
			return BLACK;
		}

		const int sx = NoaFastAbs(simple.x * h) % h;
		const int sy = NoaFastAbs(simple.y * w) % w;

		return image[sy * h + sx];
	}


	SpriteFile LoadSprFile(const char* file)
	{
		SpriteFile spriteFile;
		//spriteFile.images = nullptr;

		std::ifstream input(file, std::ios::binary);
		if (input)
		{
			uint32_t imagesCount;
			input.read(reinterpret_cast<char*>(&imagesCount), sizeof(uint32_t));

			//spriteFile.images = new uint32_t[imagesCount];
			for (int i = 0; i < imagesCount; ++i)
			{
				uint32_t imageValue;
				input.read(reinterpret_cast<char*>(&imageValue), sizeof(uint32_t));
				spriteFile.images.push_back(imageValue);
				//input.read(reinterpret_cast<char*>(&spriteFile.images[i]), sizeof(uint32_t));
			}

			input.read(reinterpret_cast<char*>(&spriteFile.x), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.y), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.width), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.height), sizeof(int));

			input.close();
		}
		else
		{
			Debug("读取失败：" + string(file));
			//std::cerr << "读取失败：" << file << std::endl;
		}

		return spriteFile;
	}
}

