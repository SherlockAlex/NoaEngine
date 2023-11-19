#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#include "Debug.h"
#include "Resource.h"
#include "GLRenderer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "Graphic.h"

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


}

void noa::GLRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void noa::GLRenderer::DrawTexture(
    noa::Texture* tex
    , int index
    , int x, int y  //绘制图片到屏幕
    , int w, int h  //图片占屏幕大小
    , unsigned int tint
    , float eulerAngle
    , bool isFlipX)
{

    ////正常一个物件一个VAO

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

    const float left = 2.0f * x * invPixelWidth - 1.0f;
    const float right = 2.0f * (x + w) * invPixelWidth - 1.0f;
    const float bottom = -(2.0f * y * invPixelHeight - 1.0f);
    const float top = -(2.0f * (y + h) * invPixelHeight - 1.0f);

    float vertices[] = {
         //顶点坐标        纹理坐标
         right,  bottom,    1.0f, 0.0f, // 右下角
         right,  top,       1.0f, 1.0f, // 右上角
         left,   top,       0.0f, 1.0f, // 左上角
         left,   bottom,    0.0f, 0.0f  // 左下角
    };

    //图片发生旋转的时候，改变的是纹理坐标

    if (isFlipX)
    {
        vertices[2] = 0.0;
        vertices[6] = 0.0;
        vertices[10] = 1.0;
        vertices[14] = 1.0;
    }

    texture->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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