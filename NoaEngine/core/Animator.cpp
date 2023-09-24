#include "Animator.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {

	extern float deltaTime;

	//vector<Animator*> animatorList;

	Animation::Animation(Actor* actor):ActorComponent(actor)
	{
	}

	Animation::Animation(Actor* actor,float speed,bool loop) :ActorComponent(actor)
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;


		if (this->frameData!=nullptr&&!this->frameData->framesImage.empty())
		{
			currentFrame = move(frameData->framesImage[0]);
		}

	}

	Animation::Animation(Actor* actor,float speed, bool loop, AnimationFrame* frame) :ActorComponent(actor)
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;

		this->SetFrame(frame);

		/*const AnimationFile animatorFile = move(resource.LoadAnimationFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}*/

		/*if (framesImage.size() > 0)
		{
			currentFrame = move(framesImage[0]);
		}*/

	}

	Animation* Animation::Create(Actor* actor, float speed, bool loop)
	{
		return new Animation(actor,speed, loop);
	}

	Animation* Animation::Create(Actor* actor, float speed, bool loop, AnimationFrame* frame)
	{
		return new Animation(actor,speed, loop, frame);
	}

	void Animation::Delete()
	{
		delete this;
	}

	Animation::~Animation()
	{
		//ActorComponent::~ActorComponent();
		Debug("Remove animation");
	}

	/// <summary>
	/// 从本地动画文件加载动画
	/// </summary>
	/// <param name="filePath">动画文件路径</param>
	/*void Animation::LoadFromAnimationFile(const char* filePath)
	{
		const AnimationFile animatorFile = move(resource.LoadAnimationFile(filePath));
		for (const SpriteFile & frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

	}*/

	void Animation::SetFrame(AnimationFrame* frame)
	{
		//设置Frame数据
		if (frame == nullptr)
		{
			Debug("this frame is empty");
			return;
		}
		this->frameData = frame;
		if (!frameData->framesImage.empty())
		{
			currentFrame = frameData->framesImage[0];
		}
		
	}

	/// <summary>
	/// 获取当前帧的图像
	/// </summary>
	/// <returns></returns>
	SpriteFile& Animation::GetCurrentFrameImage()
	{
		return currentFrame;
	}

	/// <summary>
	/// 获取指定帧的图像
	/// </summary>
	/// <param name="frame">第frame帧</param>
	/// <returns></returns>
	SpriteFile& Animation::GetFrameImage(int frame)
	{
		if (frameData == nullptr)
		{
			Debug("this frame is empty");
			exit(-1);
		}
		frame = frame %frameData->framesImage.size();
		return frameData->framesImage[frame];
	}

	void Animation::SetFrameEvent(int frame, function<void()> e)
	{
		//设置帧事件
		this->framesEvent[frame] += e;
	}

	/// <summary>
	/// 播放动画化
	/// </summary>
	/// <param name="frame">播放第frame帧的图像</param>
	void Animation::Play(int frame)
	{
		if (this->frameData == nullptr||this->frameData->framesImage.empty())
		{
			return;
		}
		currentFrame = GetFrameImage(frame);
	}

	void Animation::Play() 
	{
		if (isPlaying)
		{
			return;
		}
		i = 0;
		isPlaying = true;
	}

	/// <summary>
	/// 插入帧图像
	/// </summary>
	/// <param name="frameImage"></param>
	/*void Animation::InsertFrameImage(const SpriteFile & frameImage)
	{
		Debug("Insert Animator Frame");
		framesImage.push_back(frameImage);
		currentFrame = framesImage[0];
	}*/

	void Animation::Reset()
	{
		this->i = 0;
		this->previousFrameIndex = -1;
	}

	void Animation::Start()
	{

	}

	/// <summary>
	/// 实时更新刷新
	/// </summary>
	void Animation::Update() {
		if (!isPlaying||this->frameData == nullptr)
		{
			return;
		}

		i += deltaTime * speed;
		if (i>=this->frameData->framesImage.size())
		{
			i = 0;
			previousFrameIndex = -1;
		}
		if (!loop&&i==0)
		{
			isPlaying = false;
		}

		this->Play(i);

		//当前帧事件只执行一次
		const bool isFrameStart = (previousFrameIndex!=int(i));

		if (isFrameStart && (!framesEvent.empty()))
		{
			previousFrameIndex = int(i);
			framesEvent[i].Invoke();
		}

	}


	AnimationClip::AnimationClip(Animator* animator, Animation* animation):State(animator)
	{
		if (animator==nullptr)
		{
			Debug("init animator failed");
			exit(-1);
		}
		this->animtion = animation;
		sprite = animator->sprite;
	}

	void AnimationClip::OnEnter()
	{
		animtion->Reset();
	}

	void AnimationClip::OnUpdate()
	{
		if (sprite==nullptr) 
		{
			return;
		}
		sprite->UpdateImage(animtion->GetCurrentFrameImage());

	}

	void AnimationClip::Reason()
	{
		//动画状态切换
		//动画切换事件
	}

	void AnimationClip::OnExit()
	{
		animtion->Reset();
	}

	Animator::Animator(Actor* actor, Sprite* sprite):StateMachine(actor)
	{
		this->sprite = sprite;
	}

	Animator::Animator(Actor* actor, Sprite, vector<State*> stateList) :StateMachine(actor)
	{
		this->sprite = sprite;
	}

}

