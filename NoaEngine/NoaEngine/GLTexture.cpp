#ifdef _WIN64

#include "GLTexture.h"
#include "Debug.h"

namespace noa {
    GLTexture::GLTexture(int w, int h, uint32_t* pixelBuffer)
        :Texture()
    {

        this->width = w;
        this->height = h;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        
        

    }

    GLTexture::~GLTexture() {
        glDeleteTextures(1, &textureID);
    }

    void GLTexture::UpdateTexture(const uint32_t* pixelBuffer, const int width, const int height)
    {
        this->width = width;
        this->height = height;

        glBindTexture(GL_TEXTURE_2D, textureID);
        // 更新纹理数据
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

    }

    void GLTexture::Bind() {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void GLTexture::EnableAlpha()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    int GLTexture::GetWidth() {
        return width;
    }

    int GLTexture::GetHeight() {
        return height;
    }
}

#endif //_WIN64