#include "SDLRenderer.h"
#include "SDLTexture.h"
#include "NoaEngine.h"

void noa::SDLRenderer::InitRenderer()
{
	//³õÊ¼»¯Renderer
	
}

noa::Texture* noa::SDLRenderer::CreateTexture(int w, int h, uint32_t* pixelBuffer)
{
	noa::SDLTexture* texture = new SDLTexture(w,h,pixelBuffer);
	texture->CreateSDLTexture(this->sdlRenderer);

	return texture;
}

void noa::SDLRenderer::DrawTexture(Texture* tex, int index, int x, int y, int w, int h, float eulerAngle, bool isFlipX)
{
	//SDL_RendererCopy
	SDLTexture* texture = dynamic_cast<SDLTexture*>(tex);
	if (texture == nullptr)
	{
		return;
	}

	SDL_Rect srcRect;
	SDL_Rect dstRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = texture->width;
	srcRect.h = texture->height;

	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = w;
	dstRect.h = h;

	SDL_RenderCopyEx(sdlRenderer, texture->sdlTexture, &srcRect, &dstRect, eulerAngle, nullptr, static_cast<SDL_RendererFlip>(isFlipX));

}

void noa::SDLRenderer::Clear()
{

}

void noa::SDLRenderer::SetContext(SDL_Window* windows)
{
	sdlRenderer = SDL_CreateRenderer(windows,-1,SDL_RENDERER_ACCELERATED);
}

void noa::SDLRenderer::Present(SDL_Window* windows)
{
	SDL_RenderPresent(sdlRenderer);
}
