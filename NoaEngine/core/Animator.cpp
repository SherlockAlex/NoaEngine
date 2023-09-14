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
	/// �ӱ��ض����ļ����ض���
	/// </summary>
	/// <param name="filePath">�����ļ�·��</param>
	void Animation::LoadFromAnimationFile(const char* filePath)
	{
		const AnimationFile animatorFile = move(resource.LoadAnimationFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

	}

	/// <summary>
	/// ��ȡ��ǰ֡��ͼ��
	/// </summary>
	/// <returns></returns>
	SpriteFile& Animation::GetCurrentFrameImage()
	{
		return currentFrame;
	}

	/// <summary>
	/// ��ȡָ��֡��ͼ��
	/// </summary>
	/// <param name="frame">��frame֡</param>
	/// <returns></returns>
	SpriteFile& Animation::GetFrameImage(int frame)
	{
		frame = frame %framesImage.size();
		return framesImage[frame];
	}

	void Animation::SetFrameEvent(int frame, function<void()> e)
	{
		//����֡�¼�
		this->framesEvent[frame] += e;
	}

	/// <summary>
	/// ���Ŷ�����
	/// </summary>
	/// <param name="frame">���ŵ�frame֡��ͼ��</param>
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
	/// ����֡ͼ��
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
	/// ʵʱ����ˢ��
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

		//��ǰ֡�¼�ִֻ��һ��
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
		//����״̬�л�
		//�����л��¼�
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

