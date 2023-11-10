#include "GLShader.h"
#include "GLHelper.h"
#include "Debug.h"

GLShader defaultShader;

GLShader::GLShader() {
	

}

GLShader::~GLShader() {
	//ɾ��shader����
	glDeleteProgram(this->program);
}

void GLShader::Compile()
{
	//����vertex
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &this->vertextShaderSource, nullptr);
	glCompileShader(vertexShader);

	//����fragment
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &this->fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	//���ӽ�program����ɾ��vertext��fragment
	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	noa::Debug::Log("������ɫ���ɹ�");
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
