#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "GLTexture.h"
#include "Debug.h"
#include "Screen.h"

/**
* һ���������VAO��VBO��EBO���Լ�����ͼƬ��Rect
*/

noa::GLTexture::GLTexture(int w, int h, uint32_t* pixelBuffer)
    :Texture()
{

    this->width = w;
    this->height = h;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);


    // ���� VAO/VBO

    const float left = -1.0f;
    const float right = 1.0f;
    const float bottom = -1.0f;
    const float top = 1.0f;

    float vertices[] = {
         //��������        ��������
         right,  bottom,    1.0f, 0.0f, // ���½�
         right,  top,       1.0f, 1.0f, // ���Ͻ�
         left,   top,       0.0f, 1.0f, // ���Ͻ�
         left,   bottom,    0.0f, 0.0f  // ���½�
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
    
    //�󶨶�������
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
    const uint32_t* pixelBuffer
    , const int width
    , const int height)
{
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

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
GLuint noa::GLTexture::GetVAO()
{
    return this->VAO;
}

#endif //_WIN64