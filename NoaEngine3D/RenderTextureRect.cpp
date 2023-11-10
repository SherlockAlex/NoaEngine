#include "RenderTextureRect.h"
#include "GLShader.h"
#include "GLHelper.h"
#include "Debug.h"
#include "ResourceManager.h"

static float vertex[] = {
	//顶点坐标		纹理坐标
	-0.5f,0.5f,		0.0f,1.0f,
	-0.5f,-0.5f,	0.0f,0.0f,
	0.5f,-0.5f,		1.0f,0.0f,
	0.5f,0.5f,		1.0f,1.0f
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
static uint32_t textureID = 0;

static SpriteFile sprite;

void RenderTextureRect::Begin()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexs),indexs,GL_STATIC_DRAW);

	glVertexAttribPointer(
		0
		,2
		,GL_FLOAT
		,GL_FALSE
		,2*sizeof(float)			//本地一行数据由3个float数据
		,(void*)0);					//从0开始
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1
		,2
		,GL_FLOAT
		,GL_FALSE
		,2*sizeof(float)
		,(void*)2
		);
	glEnableVertexAttribArray(1);

	sprite = ResourceManager::LoadSprite("./texture.spr");

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//设置纹理属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D
		, 0
		, GL_RGBA
		, sprite.width		//是纹理宽度，不是图片宽度
		, sprite.height
		, 0
		, GL_RGBA
		, GL_UNSIGNED_BYTE
		, sprite.images.data()
	);

}

void RenderTextureRect::Tick()
{
	glBindTexture(GL_TEXTURE_2D,textureID);
	defaultShader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
