#include "Animator.h"
#include "NoaEngine.h"

namespace noa {

	extern float deltaTime;

	AnimationFile LoadAnimationFile(const char* file) {
		AnimationFile animator;

		std::ifstream inputFile(file, std::ios::binary);
		if (!inputFile) {
			Debug("Opening file: "+string(file)+" error");
			exit(-1);
		}

		int spriteCount;
		inputFile.read(reinterpret_cast<char*>(&spriteCount), sizeof(int));

		for (int i = 0; i < spriteCount; ++i) {
			SpriteFile sprite;
			int imageCount;
			inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

			for (int j = 0; j < imageCount; ++j) {
				uint32_t pixel;
				inputFile.read(reinterpret_cast<char*>(&pixel), sizeof(uint32_t));
				sprite.images.push_back(pixel);
			}

			inputFile.read(reinterpret_cast<char*>(&sprite.x), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.y), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.width), sizeof(int));
			inputFile.read(reinterpret_cast<char*>(&sprite.height), sizeof(int));

			animator.data.push_back(sprite);
		}

		inputFile.read(reinterpret_cast<char*>(&animator.posx), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.posy), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.w), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&animator.h), sizeof(int));

		inputFile.close();
		return animator;
	}

	//vector<Animator*> animatorList;

	Animation::Animation(float speed,bool loop) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;
		//animatorList.push_back(this);
		if (!framesImage.empty())
		{
			currentFrame = move(framesImage[0]);
		}
		//frameSize = framesImage.size();

	}

	Animation::Animation(float speed,bool loop, const char* filePath) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		this->loop = loop;
		//animatorList.push_back(this);

		const AnimationFile animatorFile = move(LoadAnimationFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

		if (framesImage.size() > 0)
		{
			currentFrame = move(framesImage[0]);
		}

		//frameSize = framesImage.size();

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
		const AnimationFile animatorFile = move(LoadAnimationFile(filePath));
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


	AnimationClip::AnimationClip(Animator* animator):State(animator)
	{
		if (animator==nullptr)
		{
			Debug("init animator failed");
			exit(-1);
		}
		sprite = animator->sprite;
	}

	void AnimationClip::Act()
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
	}

}

