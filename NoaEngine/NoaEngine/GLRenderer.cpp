#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "Debug.h"
#include "Resource.h"
#include "GLRenderer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "Graphic.h"

noa::Texture* noa::GLRenderer::CreateTexture(int w, int h, uint32_t* pixelBuffer)
{
    noa::Texture* texture = new noa::GLTexture(w, h, pixelBuffer);
    return texture;
}
noa::GLRenderer::GLRenderer() :noa::Renderer() {

}

noa::GLRenderer::~GLRenderer() {
    delete this->defaultShader;
}

void noa::GLRenderer::InitRenderer()
{

    if (glewInit() != GLEW_OK) {
        noa::Debug::Error("Failed to initialize GLEW");
        exit(-1);
    }
    std::string vertexFile
        = "../shader/vertex_shader.glsl";

    std::string fragmentFile
        = "../shader/fragment_shader.glsl";
    this->defaultShader = this->CreateShader(vertexFile, fragmentFile);
    this->defaultShader->UseShaderProgram();

    //����ͶӰ����
    //�������Ͻ�Ϊ0.0�����½�Ϊ��800��600��
    this->projection = glm::ortho(0.0f
        , static_cast<float>(this->pixelWidth)
        , static_cast<float>(this->pixelHeight)
        , 0.0f
        , -1.0f
        , 1.0f
    );

    this->defaultShader->SetMat4("projection", this->projection);


}

void noa::GLRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void noa::GLRenderer::DrawTexture(
    noa::Texture* tex
    , int index
    , int x, int y  //����ͼƬ����Ļ
    , int w, int h  //ͼƬռ��Ļ��С
    , unsigned int tint
    , float eulerAngle
    , bool isFlipX)
{

    ////����һ�����һ��VAO

    GLTexture* texture = tex->GetTextureAs<GLTexture>();
    if (texture == nullptr)
    {
        return;
    }

    // Shader���
    //glActiveTexture(GL_TEXTURE + index);

    //this->defaultShader->SetVec4(
    //    "tint"
    //    , GetRValue(tint)
    //    , GetGValue(tint)
    //    , GetBValue(tint)
    //    , GetAValue(tint)
    //);

    //this->defaultShader->SetFloat(
    //    "eulerAngle"
    //    , eulerAngle
    //);

    //texture->Bind();
    //const float left = 2.0f * x * invPixelWidth - 1.0f;
    //const float right = 2.0f * (x + w) * invPixelWidth - 1.0f;
    //const float bottom = -(2.0f * y * invPixelHeight - 1.0f);
    //const float top = -(2.0f * (y + h) * invPixelHeight - 1.0f);

    //float vertices[] = {
    //     ��������        ��������
    //     right,  bottom,    1.0f, 0.0f, // ���½�
    //     right,  top,       1.0f, 1.0f, // ���Ͻ�
    //     left,   top,       0.0f, 1.0f, // ���Ͻ�
    //     left,   bottom,    0.0f, 0.0f  // ���½�
    //};

    //ͼƬ������ת��ʱ�򣬸ı������������

    //if (isFlipX)
    //{
    //    vertices[2] = 0.0;
    //    vertices[6] = 0.0;
    //    vertices[10] = 1.0;
    //    vertices[14] = 1.0;
    //}

    //

    // ���¶��㻺��������
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // ������������������
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //������ʹ��glm�Ĵ���

    this->defaultShader->UseShaderProgram();
    
    this->defaultShader->SetInt("image", index);
    glActiveTexture(GL_TEXTURE + index);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(x, y, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * w, 0.5f * h, 0.0f));
    model = glm::rotate(model, eulerAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * w, -0.5f * h, 0.0f));

    model = glm::scale(model, glm::vec3(w,h,1.0f));

    //����Matrix4��Shader�е�model
    //����tint��Shader

    this->defaultShader->SetMat4("model",model);
    this->defaultShader->SetVec4(
        "tint"
        , GetRValue(tint)
        , GetGValue(tint)
        , GetBValue(tint)
        , GetAValue(tint)
    );

    
    
    texture->Bind();

    glBindVertexArray(texture->GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


}

void noa::GLRenderer::SetContext(uint32_t windowID)
{
    SDL_Window* window = SDL_GetWindowFromID(windowID);
    if (window == nullptr) 
    {
        Debug::Error("Get SDL Window from id error");
        exit(-1);
    }
    this->context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        Debug::Error("Create GL Context failed");
        exit(-1);
    }
    SDL_GL_SetSwapInterval(0);
}

void noa::GLRenderer::Present(uint32_t windowID)
{
    SDL_Window* window = SDL_GetWindowFromID(windowID);
    if (window == nullptr)
    {
        Debug::Error("Get SDL Window from id error");
        exit(-1);
    }
    SDL_GL_SwapWindow(window);
}

noa::GLShader* noa::GLRenderer::CreateShader(
    const std::string& vertexSourceFile
    , const std::string& fragmentSourceFile
)
{
    GLShader* shader = new GLShader();
    shader->CreateShaderProgram(vertexSourceFile, fragmentSourceFile);
    return shader;
}

#endif //_WIN64