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

	};
}

#endif // !NOAENGINE_GLSHADER_H



