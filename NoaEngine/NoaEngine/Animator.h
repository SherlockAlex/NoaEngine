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
#include "Resource.h"
#include "ActorComponent.h"
#include "StateMachine.h"


namespace noa 
{
	typedef unsigned int Uint32;

	class Actor;

	typedef struct AnimationClip 
	{
		std::vector<SpriteFile> framesImage;
		AnimationClip(const char* filePath);
	}AnimationClip;

	//可以作为一个组件挂载到角色身上，让角色播放动画
	class Animation:public ActorComponent
	{
		ACTOR_COMPONENT(Animation)
	private:
		AnimationClip* clip = nullptr;
		std::unordered_map<Uint32, NoaEvent<void>> framesEvent;
		SpriteFile currentFrame;
		bool isPlaying = false;
		float speed = 7;
		float i = 0;
		bool loop = false;
		int previousFrameIndex = -1;
		Sprite* animatedSprite = nullptr;

	protected:
		Animation(Actor * actor);
		~Animation();

	public:
		static Animation* Create(Actor* actor);
		
		
		SpriteFile& GetCurrentFrameImage();
		SpriteFile& GetFrameImage(int frame);
		
		void Play(int frame);
		void Play();
		void Reset();
		
		void Start() override;
		void Update() override;//更新动画帧

		Animation& SetLoop(bool value);
		Animation& SetSpeed(float value);
		Animation& SetFrameEvent(int frame, std::function<void()> e);
		Animation& SetAnimatedSprite(Sprite * sprite);
		Animation& SetClip(AnimationClip* frame);
		Animation* Apply();
	};

}

#endif
