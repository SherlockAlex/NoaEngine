#ifndef NOAENGINE_ANIMATOR_H
#define NOAENGINE_ANIMATOR_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>
#include <fstream>
#include <string>

#include "Sprite.h"

using namespace std;

typedef unsigned int Uint32;
typedef void(*eventFunc)(void);

//���������⣬�����ع�

typedef struct AnimatorFile {
	vector<SpriteFile> data;
	int posx;
	int posy;
	int w;
	int h;
}AnimatorFile;

extern AnimatorFile LoadAnimatorFile(const char* file);

//������
class Animator
{
private:
	vector<SpriteFile> framesImage;
	unordered_map<Uint32, eventFunc> framesEvent;
	SpriteFile* currentFrame;
	bool isPlaying = false;
	//���������ٶ�
	float speed = 7;
	float i = 0;

public:
	Animator(float speed);
	/*Animator(const char* amtFilePath,float speed);
	Animator(AnimatorFile animatorFile,float speed);
	Animator(Uint32* frameImage, float speed);*/
	~Animator();

public:
	void LoadFromAnimatorFile(const char* filePath);
	SpriteFile* GetCurrentFrameImage();
	SpriteFile* GetFrameImage(int frame);
	void SetFrameEvent(int frame, eventFunc e);
	void Play(int frame);
	void Play();
	void InsertFrameImage(SpriteFile frameImage);
	/*Uint32 GetCurrentFramePixel(int index);*/
	void Update(float deltaTime);//���¶���֡

};

#endif
