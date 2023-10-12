#ifndef NOAENGINE_SDLRENDERER_H
#define NOAENGINE_SDLRENDERER_H

#include "SDLHelper.h"
#include "Renderer.h"

namespace noa {
	class SDLRenderer:public Renderer
	{
	public:
		void InitRenderer() override;
		Texture* CreateTexture(int w, int h, uint32_t* pixelBuffer) override;
		void DrawTexture(Texture* texture, int index, int x, int y, int w, int h, float eulerAngle = 0, bool isFlipX = false) override;
		void Clear() override;
		void SetContext(SDL_Window* windows) override;
		void Present(SDL_Window* windows) override;

	private:
		SDL_Renderer* sdlRenderer = nullptr;

	};
}

#endif // NOAENGINE_SDLRENDERER_H