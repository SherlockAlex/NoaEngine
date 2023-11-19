#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#ifndef NOAENGINE_GLTEXTURE_H
#define NOAENGINE_GLTEXTURE_H

#include "GLHelper.h"
#include "Texture.h"

namespace noa {
	class GLTexture :public Texture
	{
	public:
		GLTexture(int w, int h, void* pixelBuffer);
		~GLTexture();

		void UpdateTexture(const void* pixelBuffer, const int width, const int height) override;
		void Bind();
		void EnableAlpha() override;
		int GetWidth();
		int GetHeight();

		GLuint GetVAO();

	private:

		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		GLuint textureID = 0;
		
	};
}

#endif // !NOAENGINE_GLTEXTURE_H

#endif //_WIN64

