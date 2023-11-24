#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "GLTexture.h"
#include "Debug.h"
#include "Screen.h"

/**
* 一张纹理包括VAO，VBO，EBO，以及纹理图片和Rect
*/

noa::GLTexture::GLTexture(int w, int h, void* pixelBuffer)
    :Texture()
{

    this->width = w;
    this->height = h;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 配置 VAO/VBO

    const float left = -1.0f;
    const float right = 1.0f;
    const float bottom = -1.0f;
    const float top = 1.0f;

    float vertices[] = {
         //顶点坐标        纹理坐标
         right,  bottom,    1.0f, 0.0f, // 右下角
         right,  top,       1.0f, 1.0f, // 右上角
         left,   top,       0.0f, 1.0f, // 左上角
         left,   bottom,    0.0f, 0.0f  // 左下角
    };

    uint32_t indices[] = {
        0,1,3,
        1,2,3
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //绑定顶点属性
    glVertexAttribPointer(
        0
        ,4
        ,GL_FLOAT
        ,GL_FALSE
        ,4*sizeof(float)
        ,(void*)0
    );

    glEnableVertexAttribArray(0);
    
}

noa::GLTexture::~GLTexture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textureID);
}

void noa::GLTexture::UpdateTexture(
    const void* pixelBuffer
    , const int width
    , const int height)
{
    this->width = width;
    this->height = height;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixelBuffer);

}

void noa::GLTexture::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
}

void noa::GLTexture::EnableAlpha()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int noa::GLTexture::GetWidth() {
    return width;
}

int noa::GLTexture::GetHeight() {
    return height;
}
#endif //_WIN64