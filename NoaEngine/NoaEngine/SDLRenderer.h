#ifndef NOAENGINE_SDLRENDERER_H
#define NOAENGINE_SDLRENDERER_H

#include "SDLHelper.h"
#include "Renderer.h"

namespace noa {
	class SDLRenderer:public Renderer
	{
	public:
		~SDLRenderer();

		void InitRenderer() override;
		Texture* CreateTexture(int w, int h, void* pixelBuffer) override;
		void DrawTexture(
			Texture* texture
			, int index
			, int x
			, int y
			, int w
			, int h
			, unsigned int tint
			, float eulerAngle
			, bool isFlipX) override;
		void Clear() override;
		void SetContext(uint32_t windowID) override;
		void Present(uint32_t windows) override;

	private:
		SDL_Renderer* sdlRenderer = nullptr;

	};
}

#endif // NOAENGINE_SDLRENDERER_H