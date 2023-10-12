#include "SDLTexture.h"

noa::SDLTexture::SDLTexture(int w, int h, uint32_t* pixelBuffer)
{
	this->width = w;
	this->height = h;
	this->pixelBuffer = pixelBuffer;
}

noa::SDLTexture::~SDLTexture()
{

}

void noa::SDLTexture::CreateSDLTexture(SDL_Renderer* sdlRenderer)
{
	this->sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);
	SDL_UnlockTexture(sdlTexture);
	SDL_UpdateTexture(sdlTexture, nullptr, this->pixelBuffer, this->width * sizeof(uint32_t));
}

void noa::SDLTexture::UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height)
{
	this->width = width;
	this->height = height;
	this->pixelBuffer = (uint32_t*)(pixelBuffer);

	SDL_Rect rect = { 0,0,width,height };
	SDL_UnlockTexture(sdlTexture);
	SDL_UpdateTexture(sdlTexture, &rect, pixelBuffer, width * sizeof(uint32_t));
}

void noa::SDLTexture::EnableAlpha()
{
	
}
