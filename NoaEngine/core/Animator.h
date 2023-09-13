#ifndef NOAENGINE_ANIMATOR_H
#define NOAENGINE_ANIMATOR_H

#include <iostream>
#include <functional>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>
#include <fstream>
#include <string>

#include "Sprite.h"
#include "NoaAction.h"
#include "Behaviour.h"
#include "StateMachine.h"
#include "Resource.h"

using namespace std;

namespace noa {
	typedef unsigned int Uint32;

	//���������⣬�����ع�

	//������
	class Animation:public Behaviour
	{
	private:
		vector<SpriteFile> framesImage;
		unordered_map<Uint32,NoaEvent<void()>> framesEvent;
		//unordered_map<Uint32, function<void()>> framesEvent;
		SpriteFile currentFrame;
		bool isPlaying = false;
		//���������ٶ�
		float speed = 7;
		float i = 0;
		bool loop = false;
		int previousFrameIndex = -1;

	public:
		Animation(float speed,bool loop);
		Animation(float speed,bool loop, const char* filePath);
		~Animation();

	public:
		void LoadFromAnimationFile(const char* filePath);
		SpriteFile& GetCurrentFrameImage();
		SpriteFile& GetFrameImage(int frame);
		void SetFrameEvent(int frame, function<void()> e);
		void Play(int frame);
		void Play();
		void InsertFrameImage(SpriteFile frameImage);
		void Reset();
		
		void Start() override;
		void Update() override;//���¶���֡

	};

	class Animator;

	class AnimationClip:public State
	{
	private:
		Animation* animtion = nullptr;
		Sprite* sprite = nullptr;
	public:
		AnimationClip(Animator* animator);
		void OnUpdate() override;
		void Reason() override;
	};

	//����״̬��
	class Animator:public StateMachine
	{
	public:
		Sprite* sprite = nullptr;
		
	};

}

#endif
