#ifndef NOAENGINE_TEXTURE_H
#define NOAENGINE_TEXTURE_H

namespace noa {
	typedef unsigned int uint32_t;

	class Texture
	{
	public:
		int width;
		int height;
	protected:
		Texture();
	public:
		virtual ~Texture();

		virtual void UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height) = 0;
		virtual void EnableAlpha() = 0;
	};
}


#endif


