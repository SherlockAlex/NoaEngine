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

	//����������
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//���ö�������
	glVertexAttribPointer(
		0						//��ʼ����������0
		,3						//������ɫ��location 0 ��һ��vec3��������3
		,GL_FLOAT				//��������float��
		,GL_FALSE				//�ر����ݱ�׼��
		,3*sizeof(float)		//����һ��������3��float����
		,(void*)0				//��ʼλ�õ�ƫ����
	);
	glEnableVertexAttribArray(0);//���ö�������0����glsl�е�location 0

}

void RenderTriangle::Tick()
{
	//ʹ��Ĭ����Ⱦ��
	//������һ��ģ��һ��VAO����ֻҪ����VAO,�Ϳ��Է��ʶ�Ӧ�Ķ������ݲ�����
	defaultShader.Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
}
