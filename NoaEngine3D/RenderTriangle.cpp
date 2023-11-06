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
	//����������
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DRAW_BUFFER);
	
	//���ö�������
	glVertexAttribPointer(
		0						//��ʼ����������0
		,3						//һ��3������
		,GL_FLOAT				//��������float��
		,GL_FALSE				//�ر����ݱ�׼��
		,3*sizeof(float)		//ÿ�������ƫ����
		,(void*)0				//��ʼλ�õ�ƫ����
	);
	glEnableVertexAttribArray(0);//���ö�������0����glsl�е�location 0

}

void RenderTriangle::Tick()
{
	
}
