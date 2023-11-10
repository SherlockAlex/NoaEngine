#include "GLShader.h"
#include "GLHelper.h"
#include "Debug.h"

GLShader defaultShader;

GLShader::GLShader() {
	

}

GLShader::~GLShader() {
	//删除shader程序
	glDeleteProgram(this->program);
}

void GLShader::Compile()
{
	//编译vertex
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &this->vertextShaderSource, nullptr);
	glCompileShader(vertexShader);

	//编译fragment
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &this->fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	//链接进program，并删除vertext和fragment
	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	noa::Debug::Log("编译着色器成功");
}

void GLShader::Use()
{
	glUseProgram(this->program);
}

GLShader& GLShader::SetVec4f(const std::string& name, const Vector4& vec4)
{
	const uint32_t location = glGetUniformLocation(this->program,name.c_str());
	glUniform4f(location,vec4.x,vec4.y,vec4.z,vec4.w);
	return *this;
}
