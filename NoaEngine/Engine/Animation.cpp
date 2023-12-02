#include "Animation.h"
#include "Actor.h"
#include "Time.h"
#include "SpriteRenderer.h"

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

noa::Animation& noa::Animation::SetClip(noa::AnimationClip* clip)
{
	if (clip == nullptr)
	{
		return *this;
	}
	this->clip = clip;
	if (!clip->framesImage.empty())
	{
		currentFrame = clip->framesImage[0];
	}
	this->Reset();
	return *this;

}

noa::Animation& noa::Animation::SetClip(
	const std::string& name) {
	if (this->clips.count(name)<=0) 
	{
		return *this;
	}
	this->SetClip(clips[name]);
}

noa::Animation& noa::Animation::AddClip(
	const std::string& name,noa::AnimationClip* clip) {
	if (clip == nullptr) 
	{
		return *this;
	}
	this->clips.insert({name,clip});
	return *this;
}

noa::Animation& noa::Animation::RemoveClip(
	const std::string& name) 
{
	if (this->clips.count(name)<=0) 
	{
		return *this;
	}
	auto it = clips.find(name);
	clips.erase(it);
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

noa::Animation& noa::Animation::SetFrameEvent(int frame, std::function<void()> e)
{
	this->framesEvent[frame] += e;
	return *this;
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

	if (!isPlaying ||clip == nullptr)
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

	if (spriteRenderer)
	{
		spriteRenderer->UpdateSprite(this->currentFrame);
	}

	//当前帧事件只执行一次
	const bool isFrameStart = (previousFrameIndex != int(i));

	if (isFrameStart && (!framesEvent.empty()))
	{
		previousFrameIndex = static_cast<int>(i);
		framesEvent[previousFrameIndex]();
	}

}

noa::Animation& noa::Animation::SetLoop(bool value)
{
	this->loop = value;
	return *this;
}

noa::Animation& noa::Animation::SetSpeed(float value)
{
	this->speed = value;
	return *this;
}

noa::Animation& noa::Animation::SetSpriteRenderer(
	noa::SpriteRenderer* spriteRenderer)
{
	this->spriteRenderer = spriteRenderer;
	return *this;
}
