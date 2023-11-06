#include "Animator.h"
#include "Actor.h"
#include "Time.h"

using namespace std;

namespace noa {

}

noa::AnimationClip::AnimationClip(const char* filePath)
{
	const AnimationFile animatorFile = std::move(Resource::LoadAnimationClip(filePath));
	for (const SpriteFile& frame : animatorFile.data)
	{
		framesImage.push_back(frame);
	}
}

noa::Animation::Animation(noa::Actor* actor) :noa::ActorComponent(actor)
{

}

noa::Animation* noa::Animation::Create(Actor* actor)
{
	return NObject<Animation>::Create(actor);
}

noa::Animation::~Animation()
{

}

noa::Animation* noa::Animation::SetClip(noa::AnimationClip* clip)
{
	if (clip == nullptr)
	{
		return this;
	}
	this->clip = clip;
	if (!clip->framesImage.empty())
	{
		currentFrame = clip->framesImage[0];
	}
	
	return this;

}

noa::Animation* noa::Animation::Apply()
{
	return this;
}

noa::SpriteFile& noa::Animation::GetCurrentFrameImage()
{
	return currentFrame;
}

noa::SpriteFile& noa::Animation::GetFrameImage(int frame)
{
	if (clip == nullptr)
	{
		noa::Debug::Error("this frame is empty");
		exit(-1);
	}
	frame = frame % clip->framesImage.size();
	return clip->framesImage[frame];
}

noa::Animation* noa::Animation::SetFrameEvent(int frame, function<void()> e)
{
	this->framesEvent[frame] += e;
	return this;
}

void noa::Animation::Play(int frame)
{
	if (this->clip == nullptr
		|| this->clip->framesImage.empty())
	{
		return;
	}
	currentFrame = GetFrameImage(frame);
}

void noa::Animation::Play()
{
	if (isPlaying)
	{
		return;
	}
	i = 0;
	isPlaying = true;
}

void noa::Animation::Reset()
{
	this->i = 0;
	this->previousFrameIndex = -1;
}

void noa::Animation::Start()
{

}

void noa::Animation::Update() 
{

	if (!isPlaying || this->clip == nullptr)
	{
		return;
	}

	i += ((1.0f / Time::deltaTime) < 60 ? 0.0167f: Time::deltaTime) * speed;
	if (i >= this->clip->framesImage.size())
	{
		i = 0;
		previousFrameIndex = -1;
	}
	if (!loop && i == 0)
	{
		isPlaying = false;
	}

	this->Play(static_cast<int>(i));

	if (animatedSprite) 
	{
		animatedSprite->UpdateImage(this->currentFrame);
	}

	//当前帧事件只执行一次
	const bool isFrameStart = (previousFrameIndex != int(i));

	if (isFrameStart && (!framesEvent.empty()))
	{
		previousFrameIndex = static_cast<int>(i);
		framesEvent[previousFrameIndex].Invoke();
	}

}

noa::Animation* noa::Animation::SetLoop(bool value)
{
	this->loop = value;
	return this;
}

noa::Animation* noa::Animation::SetSpeed(float value)
{
	this->speed = value;
	return this;
}

noa::Animation* noa::Animation::SetAnimatedSprite(Sprite* sprite)
{
	this->animatedSprite = sprite;
	return this;
}

noa::AnimationState::AnimationState(
	noa::Animator* animator
	, noa::Animation* animation) 
	:noa::State(animator)
{
	if (animator == nullptr)
	{
		Debug::Error("init animator failed");
		exit(-1);
	}
	this->animtion = animation;
	sprite = animator->sprite;
}

void noa::AnimationState::OnEnter()
{
	animtion->Reset();
}

void noa::AnimationState::OnUpdate()
{
	if (sprite == nullptr)
	{
		return;
	}
	sprite->UpdateImage(animtion->GetCurrentFrameImage());

}

void noa::AnimationState::Reason()
{
	//动画状态切换
	//动画切换事件
}

void noa::AnimationState::OnExit()
{
	animtion->Reset();
}

noa::Animator::Animator(noa::Actor* actor, noa::Sprite* sprite) 
	:noa::StateMachine(actor)
{
	this->sprite = sprite;
}

noa::Animator::Animator(
	noa::Actor* actor
	, noa::Sprite
	, std::vector<noa::State*> stateList
) 
	:noa::StateMachine(actor)
{
	this->sprite = sprite;
}


