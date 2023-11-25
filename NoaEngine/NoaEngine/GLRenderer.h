#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

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
		Texture* CreateTexture(int w, int h, void* pixelBuffer) override;

		GLRenderer();
		~GLRenderer();

		void InitRenderer() override;
		void Clear() override;
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
		void CreateContext(uint32_t windows) override;
		void* GetContext() override;
		void Present(uint32_t windows) override;

	private:

		void* context = nullptr;

		GLShader* CreateShader(
			const std::string& vertexSourceFile
			,const std::string& fragmentSourceFile
		);



	};
}

#endif

#endif // _WIN64

