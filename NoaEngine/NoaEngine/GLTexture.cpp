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

        float vertices[16] = {
            // 顶点坐标        纹理坐标
             1.0f,  1.0f,  1.0f, 0.0f, // 右下角
             1.0f, -1.0f,  1.0f, 1.0f, // 右上角
            -1.0f, -1.0f,  0.0f, 1.0f, // 左上角
            -1.0f,  1.0f,  0.0f, 0.0f  // 左下角
        };

        unsigned int indices[6] = {
           0, 1, 3, // 第一个三角形
           1, 2, 3  // 第二个三角形
        };

        // 生成一个VAO
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STREAM_DRAW);

        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STREAM_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }

    GLTexture::~GLTexture() {
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
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
    GLuint GLTexture::GetVAO()
    {
        return this->VAO;
    }
}

#endif //_WIN64