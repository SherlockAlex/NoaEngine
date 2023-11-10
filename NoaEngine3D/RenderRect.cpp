#include "RenderRect.h"
#include "GLShader.h"
#include "GLHelper.h"

static float vertex[] = {
	-0.5f,0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	0.5f,0.5f,0.0f,
};

static uint32_t indexs[] = {
	//第一个三角形
	0,1,2,
	//第二个三角形
	0,3,2
};

static uint32_t VAO = 0;
static uint32_t VBO = 0;
static uint32_t EBO = 0;

void RenderRect::Begin()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);

	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexs),indexs,GL_STATIC_DRAW);

	//下面输入的值由顶点着色器决定
	glVertexAttribPointer(
		0						//location = 0
		, 3						//location = 0 对应的是vec3，表示要输入3个值
		, GL_FLOAT				//值是float类型
		, GL_FALSE				//输入的值未经过标准化
		, 3 * sizeof(float)		//本地顶点坐标采样3*sizeof(float)
		, (void*)0				//从本地数组的0号地址位置开始采样顶点
	);
	glEnableVertexAttribArray(0);
}

void RenderRect::Tick()
{
	defaultShader.SetVec4f("tintColor", { 255.0f,0.0f,0.0f,255.0f }).Use();
	glBindVertexArray(VAO);
	glDrawElements(
		GL_TRIANGLES		//绘制三角形模式
		, 6					//indexs一共由6个元素
		, GL_UNSIGNED_INT	//indexs的元素都是uint32_t类型的
		, 0					//indexs初始位置
	);
}
