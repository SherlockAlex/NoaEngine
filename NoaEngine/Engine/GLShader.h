#if defined(NOA_WINDOWS)

#ifndef NOAENGINE_GLSHADER_H
#define NOAENGINE_GLSHADER_H

#include <string>
#include "GLHelper.h"

#include "Core.h"

namespace noa {
	class NOA_API GLShader
	{
	private:
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

		uint32_t GetUniformLocation(const char * uniformName);

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name,float value);
		void SetUint(const std::string& name,uint32_t value);
		void SetBool(const std::string& name, bool value);
		void SetVec2(const std::string& name,float value1,float value2);
		void SetVec3(const std::string& name,float value1,float value2,float value3);
		void SetVec4(const std::string& name,float value1,float value2,float value3,float value4);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix);
	
	private:
		friend class GLRenderer;

		uint32_t vertexShader = 0;
		uint32_t fragmentShader = 0;
		uint32_t shaderProgram = 0;

	};
}

#endif // !NOAENGINE_GLSHADER_H

#endif



