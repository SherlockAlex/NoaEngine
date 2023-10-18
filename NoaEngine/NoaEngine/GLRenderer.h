#ifdef _WIN64

#ifndef NOAENGINE_GLRENDERER_H
#define NOAENGINE_GLRENDERER_H

#include "GLHelper.h"
#include "Renderer.h"

namespace noa {
	class GLRenderer :public Renderer
	{
	private:
		bool isInit = false;
	public:
		Texture* CreateTexture(int w, int h, uint32_t* pixelBuffer) override;

		GLRenderer();
		~GLRenderer();

		void InitRenderer() override;
		void Clear() override;
		void DrawTexture(Texture* texture, int index, int x, int y, int w, int h, unsigned int tint, float eulerAngle, bool isFlipX) override;
		void SetContext(SDL_Window* windows) override;
		void Present(SDL_Window* windows) override;

	private:
		SDL_GLContext context = nullptr;

		GLuint fragmentShader;
		GLuint shaderProgram;
		GLuint vertexShader;

		GLint tintLocation;
		GLint eulerAngleLocation;

		std::string vertexSrc;
		std::string fragmentSrc;

	};
}

#endif

#endif // _WIN64

