#include "RenderColoredRect.h"
#include "GLShader.h"
#include "GLHelper.h"
#include "Debug.h"

static float vertex[]{
	-0.5f,0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	0.5f,0.5f,0.0f
};

static uint32_t indexs[]{
	0,1,2,
	0,3,2
};

static uint32_t VAO = 0;
static uint32_t VBO = 0;
static uint32_t EBO = 0;

void RenderColoredRect::Begin()
{
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexs),indexs, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0						//������ɫ��location = 0
		, 3						//(location = 0) in vec3 aPos,��vec3����Ϊ3
		, GL_FLOAT				//���ض�����������
		, GL_FALSE				//�Ƿ��һ��
		, 3 * sizeof(float)		//����һ��������3��float����
		, (void*)0				//��vertexArray��0��λ��ʼ
	);

	glEnableVertexAttribArray(0);

}

void RenderColoredRect::Tick()
{	
	//����tintColor
	defaultShader.SetVec4f("tintColor", {255.0f,255.0f,255.0f,255.0f}).Use();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
