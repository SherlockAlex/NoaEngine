#pragma once
#include <iostream>
#include <string>

struct Vector2 {
	float x = 0.0f;
	float y = 0.0f;
};

struct Vector3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Vector4 
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

class GLShader
{
public:
	GLShader();
	~GLShader();

	void Compile();
	void Use();

	GLShader& SetVec4f(const std::string & name,const Vector4 & vec4);

private:
	const char* vertextShaderSource = R"glsl(
	#version 330 core
	layout (location = 0) in vec3 position;	

	
	void main()
	{
		gl_Position = vec4(position,1.0f);
	}
	)glsl";

	const char * fragmentShaderSource = R"glsl(
	#version 330 core
	out vec4 fragColor;
	
	in vec2 texCoord;
	
	uniform sampler2D image;
	uniform vec4 tintColor;
	
	void main(){
		fragColor = (1.0f/255.0f)*tintColor*texture(image,texCoord);
	}	

	)glsl";

	uint32_t program = 0;

};

extern GLShader defaultShader;

