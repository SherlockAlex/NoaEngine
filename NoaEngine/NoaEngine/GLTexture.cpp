#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "GLTexture.h"
#include "Debug.h"

noa::GLTexture::GLTexture(int w, int h, uint32_t* pixelBuffer)
    :Texture()
{

    this->width = w;
    this->height = h;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

    // �����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    //glGenerateMipmap(GL_TEXTURE_2D);

    //float vertices[16] = {
    //    // ��������        ��������
    //     1.0f,  1.0f,  1.0f, 0.0f, // ���½�
    //     1.0f, -1.0f,  1.0f, 1.0f, // ���Ͻ�
    //    -1.0f, -1.0f,  0.0f, 1.0f, // ���Ͻ�
    //    -1.0f,  1.0f,  0.0f, 0.0f  // ���½�
    //};

    //unsigned int indices[6] = {
    //   0, 1, 3, // ��һ��������
    //   1, 2, 3  // �ڶ���������
    //};
    //
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    // ���� VAO/VBO
    float vertices[] = {
        // λ��     // ����
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

noa::GLTexture::~GLTexture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    // ������������
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

}

void noa::GLTexture::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBindTexture(GL_TEXTURE_2D, textureID);

}

void noa::GLTexture::EnableAlpha()
{
    //glBindTexture(GL_TEXTURE_2D, textureID);
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
    //return this->VAO;
}

#endif //_WIN64