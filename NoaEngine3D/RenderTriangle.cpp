#include "RenderTriangle.h"
#include "GLHelper.h"
#include <cstdint>

float vertices[] = {
	-0.5f,-0.5f,0.0f
	,0.5f,-0.5f,0.0f
	,0.0f,0.5f,0.0f
};

static uint32_t VBO = 0;
static uint32_t VAO = 0;

void RenderTriangle::Begin()
{
	//绘制三角形
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DRAW_BUFFER);
	
	//设置顶点属性
	glVertexAttribPointer(
		0						//起始顶点坐标是0
		,3						//一共3个顶点
		,GL_FLOAT				//坐标数据float型
		,GL_FALSE				//关闭数据标准化
		,3*sizeof(float)		//每个坐标的偏移量
		,(void*)0				//起始位置的偏移量
	);
	glEnableVertexAttribArray(0);//启用顶点属性0，在glsl中的location 0

}

void RenderTriangle::Tick()
{
	
}
