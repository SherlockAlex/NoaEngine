#if defined(_WIN64)||defined(__WIN32)||defined(__linux__)

#ifndef NOAENGINE_GLSHADER_H
#define NOAENGINE_GLSHADER_H

#include <string>
#include "GLHelper.h"

namespace noa {
	class GLShader
	{
	private:
		friend class GLRenderer;
	private:
		GLuint vertexShader = 0;
		GLuint fragmentShader = 0;
		GLuint shaderProgram = 0;

		GLShader();
		virtual ~GLShader();
	private:
		void CompileVertextShader(const std::string & filePath);
		void CompileFragmentShader(const std::string & filePath);
		void LinkShaderToProgram();

	public:

		void UseShaderProgram();

		void CreateShaderProgram(
			const std::string & vertexShaderFile
			,const std::string & fragmentShaderFile
		);

		GLint GetUniformLocation(const char * uniformName);

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name,float value);
		void SetUint(const std::string& name,uint32_t value);
		void SetBool(const std::string& name, bool value);
		void SetVec2(const std::string& name,float value1,float value2);
		void SetVec3(const std::string& name,float value1,float value2,float value3);
		void SetVec4(const std::string& name,float value1,float value2,float value3,float value4);

	};
}

#endif // !NOAENGINE_GLSHADER_H

#endif



