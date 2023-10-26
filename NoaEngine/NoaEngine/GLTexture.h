#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#ifndef NOAENGINE_GLTEXTURE_H
#define NOAENGINE_GLTEXTURE_H

#include "GLHelper.h"
#include "Texture.h"

namespace noa {
	class GLTexture :public Texture
	{
	public:
		GLTexture(int w, int h, uint32_t* pixelBuffer);
		~GLTexture();

		void UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height) override;
		void Bind();
		void EnableAlpha() override;
		int GetWidth();
		int GetHeight();

		GLuint GetVAO();

	private:
		GLuint textureID;
		GLuint VAO,VBO,EBO;
	};
}

#endif // !NOAENGINE_GLTEXTURE_H

#endif //_WIN64

