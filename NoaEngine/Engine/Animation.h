#ifndef NOAENGINE_ANIMATOR_H
#define NOAENGINE_ANIMATOR_H

#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>

#include "Sprite.h"
#include "NoaAction.h"
#include "Resource.h"
#include "ActorComponent.h"
#include "StateMachine.h"
#include "Core.h"

namespace noa 
{

	class NOA_API SpriteRenderer;
	class NOA_API Actor;

	typedef struct NOA_API AnimationClip
	{
		std::vector<SpriteFile> framesImage;
		AnimationClip(const char* filePath);
	}AnimationClip;

	//可以作为一个组件挂载到角色身上，让角色播放动画
	class NOA_API Animation final:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(Animation)
	private:
		Animation(Actor * actor);
		~Animation();

		void Play(int frame);
		void Start() override;
		void Update() override;//更新动画帧

	public:
		static Animation* Create(Actor* actor);
		
		SpriteFile& GetCurrentFrameImage();
		SpriteFile& GetFrameImage(int frame);
		
		
		void Play();
		void Reset();

		Animation& SetLoop(bool value);
		Animation& SetSpeed(float value);
		Animation& SetFrameEvent(int frame, std::function<void()> e);
		Animation& SetSpriteRenderer(SpriteRenderer* spriteRenderer);
		Animation& SetClip(AnimationClip* clip);
		Animation& SetClip(const std::string& name);
		Animation& AddClip(const std::string& name,AnimationClip* clip);
		Animation& RemoveClip(const std::string& name);
		Animation* Apply();

	private:
		AnimationClip* clip = nullptr;
		std::map<const std::string,noa::AnimationClip*> clips;

		int previousFrameIndex = -1;
		std::unordered_map<uint32_t, NoaEvent<void>> framesEvent;

		SpriteFile currentFrame;
		
		bool isPlaying = false;
		bool loop = false;

		float speed = 7;
		float i = 0;
		
		SpriteRenderer* spriteRenderer = nullptr;

	};

}

#endif
