#include "SDLRenderer.h"
#include "SDLTexture.h"
#include "Debug.h"
#include "Graphic.h"

noa::SDLRenderer::~SDLRenderer()
{
	SDL_DestroyRenderer(this->sdlRenderer);
}

void noa::SDLRenderer::InitRenderer()
{
	//初始化Renderer
	
}

noa::Texture* noa::SDLRenderer::CreateTexture(int w, int h, void* pixelBuffer)
{
	noa::SDLTexture* texture = new SDLTexture(w,h,pixelBuffer);
	texture->CreateSDLTexture(this->sdlRenderer);

	return texture;
}

void noa::SDLRenderer::DrawTexture(
	Texture* tex
	, int index
	, int x
	, int y
	, int w
	, int h
	, unsigned int tint
	, float eulerAngle
	, bool isFlipX)
{
	SDLTexture* texture = dynamic_cast<SDLTexture*>(tex);
	if (texture == nullptr)
	{
		return;
	}

	SDL_Rect srcRect = {};
	SDL_Rect dstRect = {};

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = texture->width;
	srcRect.h = texture->height;

	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = w;
	dstRect.h = h;

	const uint8_t r = GetRValue(tint);
	const uint8_t g = GetGValue(tint);
	const uint8_t b = GetBValue(tint);
	const uint8_t a = GetAValue(tint);

	SDL_SetTextureColorMod(texture->sdlTexture,r,g,b);
	SDL_SetTextureAlphaMod(texture->sdlTexture, a);

	SDL_RenderCopyEx(
		sdlRenderer
		, texture->sdlTexture
		,&srcRect
		, &dstRect
		, eulerAngle
		, nullptr
		, static_cast<SDL_RendererFlip>(isFlipX));

}

void noa::SDLRenderer::Clear()
{
	SDL_RenderClear(sdlRenderer);
}

void noa::SDLRenderer::CreateContext(uint32_t windowID)
{
	SDL_Window* window = SDL_GetWindowFromID(windowID);
	if (window == nullptr)
	{
		Debug::Error("Get SDL Window from id error");
		exit(-1);
	}
	//创建一个SDLRenderer
	sdlRenderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
}

void* noa::SDLRenderer::GetContext() {
	return this->sdlRenderer;
}

void noa::SDLRenderer::Present(uint32_t windowID)
{
	
	SDL_RenderPresent(sdlRenderer);
	
}
