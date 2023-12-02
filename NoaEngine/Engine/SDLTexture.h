#ifndef NOAENGINE_SDLTEXTURE_H
#define NOAENGINE_SDLTEXTURE_H

#include "SDLHelper.h"
#include "Texture.h"
#include "Core.h"


namespace noa {
	class NOA_API SDLTexture:public Texture
	{
	public:
		SDLTexture(int w, int h, void* pixelBuffer);
		~SDLTexture();

		void CreateSDLTexture(SDL_Renderer * sdlRenderer);

		void UpdateTexture(const void* pixelBuffer, const int width, const int height) override;
		void EnableAlpha() override;
	public:
		SDL_Texture* sdlTexture = nullptr;
	private:
		void* pixelBuffer = nullptr;
		SDL_Renderer* sdlRenderer = nullptr;

	};
}

#endif // !NOAENGINE_SDLTEXTURE_H



