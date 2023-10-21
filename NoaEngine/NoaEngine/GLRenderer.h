#ifdef _WIN64

#ifndef NOAENGINE_GLRENDERER_H
#define NOAENGINE_GLRENDERER_H

#include "GLHelper.h"
#include "Renderer.h"
#include "GLShader.h"

namespace noa {

	class GLShader;
	class GLRenderer :public Renderer
	{
	private:
		bool isInit = false;
		GLShader* defaultShader = nullptr;
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

		GLShader* CreateShader(
			const std::string& vertexSourceFile
			,const std::string& fragmentSourceFile
		);

		GLint tintLocation;
		GLint eulerAngleLocation;

	};
}

#endif

#endif // _WIN64

