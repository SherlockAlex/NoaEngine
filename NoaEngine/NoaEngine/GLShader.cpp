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
		Debug::Error("Compile vertex shader:" + std::string(infoLog));
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
		Debug::Error("Compile fragment shader:" + std::string(infoLog));
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
		Debug::Error("Link shader:" + std::string(info));
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

void noa::GLShader::SetInt(const std::string& name, int value)
{
	const GLint location = glGetUniformLocation(this->shaderProgram,name.c_str());
	glUniform1i(location, value);
}

void noa::GLShader::SetFloat(const std::string& name, float value)
{
	const GLint location = glGetUniformLocation(this->shaderProgram,name.c_str());
	glUniform1f(location,value);
}

void noa::GLShader::SetUint(const std::string& name, uint32_t value)
{
	const GLint location = glGetUniformLocation(this->shaderProgram,name.c_str());
	glUniform1ui(location,value);
}

void noa::GLShader::SetVec2(const std::string& name, float value1, float value2)
{
	const GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
	glUniform2f(location,value1,value2);
}

void noa::GLShader::SetVec3(const std::string& name, float value1, float value2, float value3)
{
	const GLint location = glGetUniformLocation(this->shaderProgram,name.c_str());
	glUniform3f(location,value1,value2,value3);
}

void noa::GLShader::SetBool(const std::string& name, bool value)
{
	const GLint location = glGetUniformLocation(this->shaderProgram,name.c_str());
	glUniform1i(location,value);
}

void noa::GLShader::SetVec4(const std::string& name, float value1, float value2, float value3, float value4)
{
	const GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
	glUniform4f(location,value1,value2,value3,value4);
}


