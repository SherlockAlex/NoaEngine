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
	/// 从本地动画文件加载动画
	/// </summary>
	/// <param name="filePath">动画文件路径</param>
	void Animation::LoadFromAnimationFile(const char* filePath)
	{
		const AnimationFile animatorFile = move(LoadAnimationFile(filePath));
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
		//动画状态切换
	}

}

