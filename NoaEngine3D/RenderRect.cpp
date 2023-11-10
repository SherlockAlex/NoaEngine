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
	//��һ��������
	0,1,2,
	//�ڶ���������
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

	//���������ֵ�ɶ�����ɫ������
	glVertexAttribPointer(
		0						//location = 0
		, 3						//location = 0 ��Ӧ����vec3����ʾҪ����3��ֵ
		, GL_FLOAT				//ֵ��float����
		, GL_FALSE				//�����ֵδ������׼��
		, 3 * sizeof(float)		//���ض����������3*sizeof(float)
		, (void*)0				//�ӱ��������0�ŵ�ַλ�ÿ�ʼ��������
	);
	glEnableVertexAttribArray(0);
}

void RenderRect::Tick()
{
	defaultShader.SetVec4f("tintColor", { 255.0f,0.0f,0.0f,255.0f }).Use();
	glBindVertexArray(VAO);
	glDrawElements(
		GL_TRIANGLES		//����������ģʽ
		, 6					//indexsһ����6��Ԫ��
		, GL_UNSIGNED_INT	//indexs��Ԫ�ض���uint32_t���͵�
		, 0					//indexs��ʼλ��
	);
}
