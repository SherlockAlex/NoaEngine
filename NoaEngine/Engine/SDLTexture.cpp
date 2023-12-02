#include "SDLTexture.h"

noa::SDLTexture::SDLTexture(int w, int h, void* pixelBuffer)
{
	this->width = w;
	this->height = h;
	this->pixelBuffer = pixelBuffer;
}

noa::SDLTexture::~SDLTexture()
{
	SDL_DestroyTexture(this->sdlTexture);
}

void noa::SDLTexture::CreateSDLTexture(SDL_Renderer* sdlRenderer)
{
	this->sdlRenderer = sdlRenderer;
	this->sdlTexture = SDL_CreateTexture(
		sdlRenderer
		, SDL_PIXELFORMAT_ABGR8888
		, SDL_TEXTUREACCESS_TARGET
		, this->width, this->height);
	SDL_UnlockTexture(sdlTexture);
	SDL_UpdateTexture(sdlTexture, nullptr, this->pixelBuffer, this->width * sizeof(uint32_t));
}

void noa::SDLTexture::UpdateTexture(const void* pixelBuffer, const int width, const int height)
{
	this->width = width;
	this->height = height;
	this->pixelBuffer = (void*)pixelBuffer;

	SDL_DestroyTexture(sdlTexture);
	this->sdlTexture = SDL_CreateTexture(
		sdlRenderer
		, SDL_PIXELFORMAT_ABGR8888
		, SDL_TEXTUREACCESS_TARGET
		, this->width, this->height);
	SDL_Rect rect = { 0,0,width,height };
	SDL_UnlockTexture(sdlTexture);
	SDL_UpdateTexture(sdlTexture, &rect, pixelBuffer, width * sizeof(uint32_t));
	this->EnableAlpha();
}

void noa::SDLTexture::EnableAlpha()
{
	SDL_SetTextureBlendMode(this->sdlTexture, SDL_BLENDMODE_BLEND);
}
