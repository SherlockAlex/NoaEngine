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
#include "ActorComponent.h"

namespace noa {
	typedef unsigned int Uint32;

	//出现了问题，必须重构

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

	//动画器
	class Animation:public ActorComponent
	{
	private:
		//framesImage可以通过复用降低内存消耗
		//vector<SpriteFile> framesImage;
		AnimationFrame* frameData = nullptr;
		std::unordered_map<Uint32,NoaEvent<void()>> framesEvent;
		//unordered_map<Uint32, function<void()>> framesEvent;
		SpriteFile currentFrame;
		bool isPlaying = false;
		//动画播放速度
		float speed = 7;
		float i = 0;
		bool loop = false;
		int previousFrameIndex = -1;

	protected:
		Animation(Actor * actor);
		Animation(Actor* actor,float speed,bool loop);
		Animation(Actor* actor,float speed,bool loop, AnimationFrame* frame);
		~Animation();

	public:
		static Animation* Create(Actor* actor, float speed, bool loop);
		static Animation* Create(Actor* actor, float speed, bool loop, AnimationFrame* frame);
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
		void Update() override;//更新动画帧

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

	//动画状态机
	class Animator:public StateMachine
	{
	public:
		Sprite* sprite = nullptr;
	public:
		Animator(Actor * actor,Sprite * sprite);
		Animator(Actor* actor, Sprite, std::vector<State*> stateList);

	};

}

#endif
