#include "Animator.h"
#include "NoaEngine.h"

namespace noa {

	extern float deltaTime;

	//vector<Animator*> animatorList;

	Animation::Animation(float speed,bool loop) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;

		if (!framesImage.empty())
		{
			currentFrame = move(framesImage[0]);
		}

	}

	Animation::Animation(float speed,bool loop, const char* filePath) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;

		const AnimationFile animatorFile = move(resource.LoadAnimationFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

		if (framesImage.size() > 0)
		{
			currentFrame = move(framesImage[0]);
		}

	}

	Animation::~Animation()
	{
		Behaviour::~Behaviour();
	}

	/// <summary>
	/// 从本地动画文件加载动画
	/// </summary>
	/// <param name="filePath">动画文件路径</param>
	void Animation::LoadFromAnimationFile(const char* filePath)
	{
		const AnimationFile animatorFile = move(resource.LoadAnimationFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
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
		frame = frame %framesImage.size();
		return framesImage[frame];
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
		if (framesImage.empty())
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
	void Animation::InsertFrameImage(SpriteFile frameImage)
	{
		Debug("Insert Animator Frame");
		framesImage.push_back(frameImage);
		currentFrame = framesImage[0];
	}

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
		if (!isPlaying)
		{
			return;
		}

		i += deltaTime * speed;
		if (i>=framesImage.size())
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

	Animator::Animator(Sprite* sprite):StateMachine()
	{
		this->sprite = sprite;
	}

	Animator::Animator(Sprite, vector<State*> stateList):StateMachine(stateList)
	{
		this->sprite = sprite;
	}

}

