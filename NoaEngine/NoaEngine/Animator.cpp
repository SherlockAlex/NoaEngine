#include "Animator.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {

}

noa::Animation::Animation(noa::Actor* actor) :noa::ActorComponent(actor)
{

}

noa::Animation::Animation(noa::Actor* actor, float speed, bool loop) :noa::ActorComponent(actor)
{
	this->speed = speed;
	this->loop = loop;


	if (this->frameData != nullptr && !this->frameData->framesImage.empty())
	{
		currentFrame = move(frameData->framesImage[0]);
	}

}

noa::Animation::Animation(noa::Actor* actor, float speed, bool loop, noa::AnimationFrame* frame) :noa::ActorComponent(actor)
{
	this->speed = speed;
	this->loop = loop;

	this->SetFrame(frame);

}

noa::Animation* noa::Animation::Create(noa::Actor* actor, float speed, bool loop)
{
	return new noa::Animation(actor, speed, loop);
}

noa::Animation* noa::Animation::Create(noa::Actor* actor, float speed, bool loop, noa::AnimationFrame* frame)
{
	return new noa::Animation(actor, speed, loop, frame);
}

noa::Animation::~Animation()
{

}

void noa::Animation::SetFrame(noa::AnimationFrame* frame)
{
	if (frame == nullptr)
	{
		return;
	}
	this->frameData = frame;
	if (!frameData->framesImage.empty())
	{
		currentFrame = frameData->framesImage[0];
	}

}

noa::SpriteFile& noa::Animation::GetCurrentFrameImage()
{
	return currentFrame;
}

noa::SpriteFile& noa::Animation::GetFrameImage(int frame)
{
	if (frameData == nullptr)
	{
		noa::Debug::Error("this frame is empty");
		exit(-1);
	}
	frame = frame % frameData->framesImage.size();
	return frameData->framesImage[frame];
}

void noa::Animation::SetFrameEvent(int frame, function<void()> e)
{
	this->framesEvent[frame] += e;
}

void noa::Animation::Play(int frame)
{
	if (this->frameData == nullptr || this->frameData->framesImage.empty())
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

	if (!isPlaying || this->frameData == nullptr)
	{
		return;
	}

	i += ((1.0f / Time::deltaTime) < 60 ? 0.0167f: Time::deltaTime) * speed;
	if (i >= this->frameData->framesImage.size())
	{
		i = 0;
		previousFrameIndex = -1;
	}
	if (!loop && i == 0)
	{
		isPlaying = false;
	}

	this->Play(static_cast<int>(i));

	//当前帧事件只执行一次
	const bool isFrameStart = (previousFrameIndex != int(i));

	if (isFrameStart && (!framesEvent.empty()))
	{
		previousFrameIndex = static_cast<int>(i);
		framesEvent[previousFrameIndex].Invoke();
	}

}

noa::AnimationClip::AnimationClip(noa::Animator* animator, noa::Animation* animation) :noa::State(animator)
{
	if (animator == nullptr)
	{
		Debug::Error("init animator failed");
		exit(-1);
	}
	this->animtion = animation;
	sprite = animator->sprite;
}

void noa::AnimationClip::OnEnter()
{
	animtion->Reset();
}

void noa::AnimationClip::OnUpdate()
{
	if (sprite == nullptr)
	{
		return;
	}
	sprite->UpdateImage(animtion->GetCurrentFrameImage());

}

void noa::AnimationClip::Reason()
{
	//动画状态切换
	//动画切换事件
}

void noa::AnimationClip::OnExit()
{
	animtion->Reset();
}

noa::Animator::Animator(noa::Actor* actor, noa::Sprite* sprite) :noa::StateMachine(actor)
{
	this->sprite = sprite;
}

noa::Animator::Animator(noa::Actor* actor, noa::Sprite, std::vector<noa::State*> stateList) :noa::StateMachine(actor)
{
	this->sprite = sprite;
}


