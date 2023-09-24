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
	/// �ӱ��ض����ļ����ض���
	/// </summary>
	/// <param name="filePath">�����ļ�·��</param>
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
		//����Frame����
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
		//����֡�¼�
		this->framesEvent[frame] += e;
	}

	/// <summary>
	/// ���Ŷ�����
	/// </summary>
	/// <param name="frame">���ŵ�frame֡��ͼ��</param>
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
	/// ����֡ͼ��
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
	/// ʵʱ����ˢ��
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

	Animator::Animator(Actor* actor, Sprite* sprite):StateMachine(actor)
	{
		this->sprite = sprite;
	}

	Animator::Animator(Actor* actor, Sprite, vector<State*> stateList) :StateMachine(actor)
	{
		this->sprite = sprite;
	}

}

