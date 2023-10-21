#include "GLShader.h"
#include "Resource.h"
#include "Debug.h"

noa::GLShader::GLShader() {

}

noa::GLShader::~GLShader()
{
	glDeleteProgram(this->shaderProgram);
}

void noa::GLShader::CompileVertextShader(const std::string& filePath)
{
	std::string source = std::move(noa::resource.ReadSourceFrom(filePath));
	const char* src = source.c_str();

	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShader,1,&src,nullptr);
	glCompileShader(this->vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(this->vertexShader, 512, nullptr, infoLog);
		Debug::Error(std::string(infoLog));
	}
}

void noa::GLShader::CompileFragmentShader(const std::string& filePath)
{
	std::string source = std::move(noa::resource.ReadSourceFrom(filePath));
	const char* src = source.c_str();
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragmentShader,1,&src,nullptr);
	glCompileShader(this->fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) 
	{
		glGetShaderInfoLog(this->fragmentShader,512,nullptr,infoLog);
		Debug::Error(std::string(infoLog));
	}
}

void noa::GLShader::LinkShaderToProgram()
{
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram,this->vertexShader);
	glAttachShader(this->shaderProgram, this->fragmentShader);
	glLinkProgram(this->shaderProgram);
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);

	int success;
	char info[512];
	glGetProgramiv(this->shaderProgram,GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderProgram,512,nullptr,info);
		Debug::Error(std::string(info));
	}

}

void noa::GLShader::UseShaderProgram()
{
	glUseProgram(this->shaderProgram);
}

void noa::GLShader::CreateShaderProgram(
	const std::string& vertexShaderFile
	,const std::string& fragmentShaderFile)
{
	this->CompileVertextShader(vertexShaderFile);
	this->CompileFragmentShader(fragmentShaderFile);
	this->LinkShaderToProgram();

}

GLint noa::GLShader::GetUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(this->shaderProgram, uniformName);
}


