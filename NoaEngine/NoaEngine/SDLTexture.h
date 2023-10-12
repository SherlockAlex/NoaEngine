#ifndef NOAENGINE_SDLTEXTURE_H
#define NOAENGINE_SDLTEXTURE_H

#include "SDLHelper.h"
#include "Texture.h"

namespace noa {
	class SDLTexture:public Texture
	{
	public:
		SDLTexture(int w, int h, uint32_t* pixelBuffer);
		~SDLTexture();

		void CreateSDLTexture(SDL_Renderer * sdlRenderer);

		void UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height) override;
		void EnableAlpha() override;
	public:
		SDL_Texture* sdlTexture = nullptr;

	private:
		uint32_t* pixelBuffer = nullptr;

	};
}

#endif // !NOAENGINE_SDLTEXTURE_H



