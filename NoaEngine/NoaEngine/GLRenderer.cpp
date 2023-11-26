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
    , int x, int y  //绘制图片到屏幕
    , int w, int h  //图片占屏幕大小
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

    //图像的投影矩阵绕的是角色关于世界坐标系原定继续旋转
    //但是实际我们希望绕的是玩家关于自己的坐标
    //因此还有对旋转后的坐标进行一个修正

    defaultShader->SetFloat(
        "eulerAngle"
        ,eulerAngle
    );

    //将把坐标系转为屏幕坐标系
    const float fixYLow = static_cast<float>(noa::Screen::height - y);
    const float fixYHeight = static_cast<float>(noa::Screen::height - (y + h));
    const float fixXLow = static_cast<float>(x);
    const float fixXHeight = static_cast<float>(x+w);

    float vertices[] = {
        fixXHeight,fixYLow,1.0f,0.0f
        ,fixXHeight,fixYHeight,1.0f,1.0f
        ,fixXLow,fixYHeight,0.0f,1.0f
        ,fixXLow,fixYLow,0.0f,0.0f
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

    if (!renderable)
    {
        return;
    }

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