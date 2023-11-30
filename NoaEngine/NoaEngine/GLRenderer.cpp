#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "Debug.h"
#include "Resource.h"
#include "GLRenderer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "Graphic.h"
#include "Screen.h"

noa::Texture* noa::GLRenderer::CreateTexture(int w, int h, void* pixelBuffer)
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

    glm::mat4 projection = glm::ortho(
        0.0f
        , static_cast<float>(noa::Screen::width)
        , 0.0f
        , static_cast<float>(noa::Screen::height)
    );

    this->defaultShader->SetMatrix4("projection", projection);

}

void noa::GLRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void noa::GLRenderer::DrawTexture(
    noa::Texture* tex
    , int index
    , int x, int y  //»æÖÆÍ¼Æ¬µ½ÆÁÄ»
    , int w, int h  //Í¼Æ¬Õ¼ÆÁÄ»´óÐ¡
    , unsigned int tint
    , float eulerAngle
    , bool isFlipX)
{
    GLTexture* texture = tex->GetTextureAs<GLTexture>();
    if (texture == nullptr)
    {
        return;
    }

    defaultShader->SetVec4(
        "tint"
        ,GetRValue(tint)
        ,GetGValue(tint)
        ,GetBValue(tint)
        ,GetAValue(tint)
    );

    defaultShader->SetFloat(
        "eulerAngle"
        ,eulerAngle
    );
    
    const float posX = static_cast<float>(x);
    const float posY = static_cast<float>(noa::Screen::height - y);

    const float sizeX = static_cast<float>(w);
    const float sizeY = static_cast<float>(h);

    defaultShader->SetBool("isFlipX",isFlipX);
    defaultShader->SetVec2("offset", posX,posY);
    defaultShader->SetVec2("size",sizeX,sizeY);

    texture->Bind();
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

}

void noa::GLRenderer::CreateContext(uint32_t windowID)
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

void* noa::GLRenderer::GetContext() {
    return this->context;
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