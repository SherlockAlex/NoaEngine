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
#include "Actor.h"
#include "StateMachine.h"
#include "Resource.h"

namespace noa {
	typedef unsigned int Uint32;

	//���������⣬�����ع�

	typedef struct AnimationFrame {
		std::vector<SpriteFile> framesImage;

		AnimationFrame(const char * filePath)
		{
			const AnimationFile animatorFile = std::move(resource.LoadAnimationFile(filePath));
			for (const SpriteFile& frame : animatorFile.data)
			{
				framesImage.push_back(frame);
				//InsertFrameImage(frame);
			}
		}

	}AnimationFrame;

	//������
	class Animation:public Actor
	{
	private:
		//framesImage����ͨ�����ý����ڴ�����
		//vector<SpriteFile> framesImage;
		AnimationFrame* frameData = nullptr;
		std::unordered_map<Uint32,NoaEvent<void()>> framesEvent;
		//unordered_map<Uint32, function<void()>> framesEvent;
		SpriteFile currentFrame;
		bool isPlaying = false;
		//���������ٶ�
		float speed = 7;
		float i = 0;
		bool loop = false;
		int previousFrameIndex = -1;

	protected:
		Animation(float speed,bool loop);
		Animation(float speed,bool loop, AnimationFrame* frame);
		~Animation();

	public:
		static Animation* Create(float speed, bool loop);
		static Animation* Create(float speed, bool loop, AnimationFrame* frame);
		void Delete() override;
		
		void SetFrame(AnimationFrame * frame);
		//void LoadFromAnimationFile(const char* filePath);
		SpriteFile& GetCurrentFrameImage();
		SpriteFile& GetFrameImage(int frame);
		void SetFrameEvent(int frame, std::function<void()> e);
		void Play(int frame);
		void Play();
		//void InsertFrameImage(const SpriteFile & frameImage);
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
		AnimationClip(Animator* animator,Animation* animation);
		void OnEnter() override;
		void OnUpdate() override;
		void Reason() override;
		void OnExit() override;
	};

	//����״̬��
	class Animator:public StateMachine
	{
	public:
		Sprite* sprite = nullptr;
	public:
		Animator(Sprite * sprite);
		Animator(Sprite, std::vector<State*> stateList);

	};

}

#endif
