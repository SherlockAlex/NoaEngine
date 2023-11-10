#include "RenderTriangle.h"
#include "GLHelper.h"
#include "GLShader.h"
#include <cstdint>
#include "Debug.h"

static float vertices[] = {
	-0.5f,-0.5f,0.0f
	,0.5f,-0.5f,0.0f
	,0.0f,0.5f,0.0f
};

static uint32_t VBO = 0;
static uint32_t VAO = 0;

void RenderTriangle::Begin()
{

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//绘制三角形
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//设置顶点属性
	glVertexAttribPointer(
		0						//起始顶点坐标是0
		,3						//顶点着色器location 0 是一个vec3，所以是3
		,GL_FLOAT				//坐标数据float型
		,GL_FALSE				//关闭数据标准化
		,3*sizeof(float)		//本地一行数据由3个float数据
		,(void*)0				//起始位置的偏移量
	);
	glEnableVertexAttribArray(0);//启用顶点属性0，在glsl中的location 0

}

void RenderTriangle::Tick()
{
	//使用默认渲染器
	//理论上一个模型一个VAO，你只要绑定了VAO,就可以访问对应的顶点数据并绘制
	defaultShader.Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
}
