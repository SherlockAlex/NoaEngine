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
using namespace std;

typedef unsigned int Uint32;
typedef void(*eventFunc)(void);

typedef struct AnimatorFile {
	vector<uint32_t*> data;
	int posx;
	int posy;
	int w;
	int h;
}AnimatorFile;

//������
class Animator
{
private:
	vector<Uint32*> framesImage;
	unordered_map<Uint32, eventFunc> framesEvent;
	Uint32* currentFrame;
	bool isPlaying = false;
	//���������ٶ�
	float speed = 7;
	float i = 0;

public:
	Animator(float speed);
	Animator(AnimatorFile animatorFile);
	Animator(Uint32* frameImage, float speed);
	~Animator();

public:
	void LoadFromAnimatorFile(const char* filePath);
	Uint32* GetCurrentFrameImage();
	Uint32* GetFrameImage(int frame);
	void SetFrameEvent(int frame, eventFunc e);
	void Play(int frame);
	void Play();
	void InsertFrameImage(Uint32* frameImage);
	Uint32 GetCurrentFramePixel(int index);
	void Update();//���¶���֡

};

extern vector<Animator*> animatorList;

#endif
